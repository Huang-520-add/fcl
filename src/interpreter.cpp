#include "interpreter.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <regex>
#include <sstream>
#include <thread>

#ifndef _WIN32
#include <sys/select.h>
#include <unistd.h>
#else
#include <conio.h>
#include <windows.h>
#endif

namespace fcl {

// 递归标记 inDecay（A1 修复：此前仅标顶层语句，导致 DECAY 内控制流块中的
// DEVOURS 失去"分解者豁免营养级检查"与"能量税豁免"）
static void markDecay(std::vector<Stmt>& v, bool dec) {
    for (auto& s : v) {
        s.inDecay = dec;
        if (s.hasBody) {
            markDecay(s.body, dec);
            if (s.hasDry) markDecay(s.dryBody, dec);
        }
    }
}

// 结构化校验 FOODWEB 是否含捕食行为（C2 修复：此前用子串搜索，字符串里的
// "DEVOURS" 字样会误判；改为遍历语句树）
static bool hasPredation(const std::vector<Stmt>& v) {
    for (const auto& s : v) {
        if (s.kind == "DEVOURS" || s.kind == "SCENT" || s.kind == "POUNCE") return true;
        if (s.hasBody) {
            if (hasPredation(s.body)) return true;
            if (s.hasDry && hasPredation(s.dryBody)) return true;
        }
    }
    return false;
}

// ============================================================
//  运行入口
// ============================================================
void Interp::run(const std::string& raw) {
    std::string src = stripComments(raw);
    // 全局开关（三段式之外，程序首行）：GMO / STORM / NUMERIC OUTPUT / REAL MODE
    // P2-4 修复：精确按语句匹配（此前为任意子串搜索，注释/字符串含 "GMO" 会误触发）
    size_t biomePos = src.find("BIOME");
    if (biomePos != std::string::npos) {
        std::stringstream prefixStream(src.substr(0, biomePos));
        std::string seg;
        while (std::getline(prefixStream, seg, ';')) {
            std::vector<std::string> toks = splitWS(trim(seg));
            if (toks.empty()) continue;
            if (toks.size() == 2 && toks[0] == "GMO" && toks[1] == "ENABLED") gmo_ = true;
            else if (toks.size() == 2 && toks[0] == "STORM" && toks[1] == "ENABLED") storm_ = true;
            else if (toks.size() == 2 && toks[0] == "NUMERIC" && toks[1] == "OUTPUT") numericOut_ = true;
            else if (toks.size() == 2 && toks[0] == "REAL" && toks[1] == "MODE") realMode_ = true;
            else if (toks.size() == 2 && toks[0] == "CODE" && toks[1] == "MODE") realMode_ = false;
        }
    }

    // 三段式提取（按顺序，禁止颠倒）
    // C1 修复：块结束位置用"字符串感知"的匹配花括号计算，与 extractBlock 一致，
    // 避免 '}' 落在字符串字面量内时被 src.find('}') 误判边界。
    auto findMatchingBrace = [](const std::string& s, size_t open) {
        int depth = 0; bool inStr = false;
        for (size_t i = open; i < s.size(); i++) {
            if (s[i] == '"') { inStr = !inStr; continue; }
            if (inStr) continue;
            if (s[i] == '{') depth++;
            else if (s[i] == '}') { if (--depth == 0) return i; }
        }
        return std::string::npos;
    };
    // A4 修复：错误行号改为"文件绝对行号"。注释剥离后每行仍对应一个 '\n'，
    // 故按从文件开头到块 '{' 之前的换行数即可得绝对行。
    auto lineAt = [](const std::string& s, size_t pos) {
        int ln = 1;
        for (size_t i = 0; i < pos && i < s.size(); i++) if (s[i] == '\n') ln++;
        return ln;
    };

    size_t biomeOpen = src.find('{', src.find("BIOME"));
    size_t bEnd = findMatchingBrace(src, biomeOpen);
    if (bEnd == std::string::npos) throw FclError(ErrCode::STRUCTURE, "🌍 生态崩溃，食物链断裂！");
    std::string biome = extractBlock(src, "BIOME", 0);

    size_t foodOpen = src.find('{', src.find("FOODWEB", bEnd));
    size_t fEnd = findMatchingBrace(src, foodOpen);
    if (fEnd == std::string::npos) throw FclError(ErrCode::STRUCTURE, "🌍 生态崩溃，食物链断裂！");
    std::string foodweb = extractBlock(src, "FOODWEB", bEnd);

    size_t decayOpen = src.find('{', src.find("DECAY", fEnd));
    std::string decay = extractBlock(src, "DECAY", fEnd);

    int lb = lineAt(src, biomeOpen);
    int lf = lineAt(src, foodOpen);
    int ld = lineAt(src, decayOpen);
    std::vector<Stmt> b = parseBlock(biome, lb);
    std::vector<Stmt> f = parseBlock(foodweb, lf);
    std::vector<Stmt> d = parseBlock(decay, ld);

    // C2 修复：FOODWEB 必须含捕食行为，改为结构化遍历语句树（此前为子串搜索）
    if (!hasPredation(f))
        throw FclError(ErrCode::STRUCTURE, "🌍 生态崩溃，食物链断裂！");

    // A1 修复：inDecay 递归标记到所有嵌套语句
    markDecay(b, false);
    markDecay(f, false);
    markDecay(d, true);

    biomePhase_ = true;
    execStmts(b);
    biomePhase_ = false;
    execStmts(f);
    execStmts(d);
    std::cout << "\n[FCL] 生态系统运行完毕。现存物种: " << vars_.size()
              << " | 指令总数: " << stmtCount_ << std::endl;
}

// ============================================================
//  预处理
// ============================================================
std::string Interp::stripComments(const std::string& raw) {
    std::stringstream out, in(raw);
    std::string line;
    std::regex obsRe(R"(^\s*OBSERVATION:\s*\d{4}-\d{2}-\d{2},\s*Lat:[+-]?[\d.]+,\s*Lon:[+-]?[\d.]+,.*$)");
    while (std::getline(in, line)) {
        std::string t = trim(line);
        if (t.empty()) { out << "\n"; continue; }
        if (t.rfind("OBSERVATION:", 0) == 0) {
            if (!std::regex_match(t, obsRe)) std::cout << "🔭 缺乏科考精神！" << std::endl;
            out << "\n";
        } else if (islower((unsigned char)t[0])) {
            // A8 修复：小写首字母的行仍按注释忽略，但若其首 token 恰为 FCL 关键字，
            // 极可能是关键字被小写导致整行被静默吞掉——给出提醒而非无声失败。
            // （仅看首 token，避免注释里顺带提到关键字时也刷屏警告。）
            static const char* KW[] = {
                "INTRODUCE","DEVOURS","CLONE","ASSESS","SYMBIOSIS","COMPETITION",
                "MIMICRY","ROT","SCENT","LURK","POUNCE","EXTINCTION","SEASON",
                "MIGRATION","HIBERNATION","MUTATION","CASE","GMO","STORM",
                "NUMERIC","REAL","CODE","BIOME","FOODWEB","DECAY", nullptr};
            std::vector<std::string> toks = splitWS(t);
            if (!toks.empty()) {
                std::string up = toks[0];
                for (auto& c : up) c = (char)std::toupper((unsigned char)c);
                for (const char* k = KW[0]; k != nullptr; k++) {
                    if (up == k) {
                        std::cout << "⚠️ 该行以小写开头被当作注释忽略，但首 token 是关键字 "
                                  << toks[0] << "（是否大小写拼写错误？）" << std::endl;
                        break;
                    }
                }
            }
            out << "\n";  // 小写内容视为注释
        } else {
            out << line << "\n";
        }
    }
    return out.str();
}

std::string Interp::extractBlock(const std::string& src, const std::string& name, size_t from) {
    size_t p = src.find(name, from);
    if (p == std::string::npos) throw FclError(ErrCode::STRUCTURE, "🌍 生态崩溃，食物链断裂！");
    size_t ob = src.find('{', p);
    if (ob == std::string::npos) throw FclError(ErrCode::STRUCTURE, "🌍 生态崩溃，食物链断裂！");
    int depth = 0;
    bool inStr = false;  // 字符串内的 { } 不计入嵌套（CASE "特征{...}" 安全）
    for (size_t i = ob; i < src.size(); i++) {
        if (src[i] == '"') { inStr = !inStr; continue; }
        if (inStr) continue;
        if (src[i] == '{') depth++;
        else if (src[i] == '}') {
            depth--;
            if (depth == 0) return src.substr(ob + 1, i - ob - 1);
        }
    }
    throw FclError(ErrCode::STRUCTURE, "🌍 生态崩溃，食物链断裂！");
}

// ============================================================
//  变量
// ============================================================
Variable& Interp::getVar(const std::string& name) {
    auto it = vars_.find(name);
    if (it == vars_.end()) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵，语法免疫系统失效");
    return it->second;
}

void Interp::touch(const std::string& name) {
    auto it = vars_.find(name);
    if (it != vars_.end()) { it->second.age = 0; touchedThis_.insert(name); }
}

size_t Interp::addrOf(const std::string& name) {
    // P2-6 修复：地址表 O(1) 查询（此前线性扫描 O(n)）
    auto it = addrMap_.find(name);
    return (it != addrMap_.end()) ? it->second : 0;
}

void Interp::rebuildAddrMap() {
    addrMap_.clear();
    for (size_t i = 0; i < varOrder_.size(); i++) addrMap_[varOrder_[i]] = i;
}

Trophic Interp::parseTrophic(const std::string& s) {
    if (s == "PRODUCER") return PRODUCER;
    if (s == "HERBIVORE") return HERBIVORE;
    if (s == "CARNIVORE") return CARNIVORE;
    if (s == "APEX") return APEX;
    if (s == "DECOMPOSER") return DECOMPOSER;
    throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵，语法免疫系统失效");
}

// ============================================================
//  执行框架
// ============================================================
void Interp::execStmts(std::vector<Stmt>& stmts) {
    for (auto& s : stmts) execOne(s);
}

void Interp::humidityPush(bool isDevour) {
    humidity_.push_back(isDevour);
    while (humidity_.size() > 3) humidity_.pop_front();
}

int Interp::humidityTrue() {
    return (int)std::count(humidity_.begin(), humidity_.end(), true);
}

void Interp::execOne(Stmt& s) {
    stmtCount_++;
    touchedThis_.clear();
    humidityPush(s.kind == "DEVOURS");
    try {
        if (s.hasBody) {
            blockDepth_++;
            if (s.kw == "SEASON") execSeason(s);
            else if (s.kw == "MIGRATION") execMigration(s);
            else if (s.kw == "HIBERNATION") execHibernation(s);
            else if (s.kw == "MUTATION") execMutation(s);
            else if (s.kw == "WHILE") execWhile(s);
            else if (s.kw == "CASE") execStmts(s.body);
            blockDepth_--;
        } else {
            if (s.kind == "GMO") gmo_ = true;
            else if (s.kind == "STORM") storm_ = true;
            else if (s.kind == "NUMERIC") numericOut_ = true;
            else if (s.kind == "REALMODE") realMode_ = true;
            else if (s.kind == "CODEMODE") realMode_ = false;
            else if (s.kind == "INTRODUCE") execIntroduce(s);
            else if (s.kind == "DEVOURS") execDevour(s);
            else if (s.kind == "CLONE") execClone(s);
            else if (s.kind == "ASSESS") execAssess(s);
            else if (s.kind == "SYMBIOSIS") execSymbiosis(s);
            else if (s.kind == "COMPETITION") execCompetition(s);
            else if (s.kind == "MIMICRY") execMimicry(s);
            else if (s.kind == "ROT") execRot(s);
            else if (s.kind == "SCENT") execScent(s);
            else if (s.kind == "LURK") execLurk(s);
            else if (s.kind == "POUNCE") execPounce(s);
            else if (s.kind == "EXTINCTION") execExtinction(s);
            else if (s.kind == "FORWARD") execForward(s);
            else if (s.kind == "BACKWARD") execBackward(s);
            else if (s.kind == "BUMP") execBump(s);
            else if (s.kind == "LOAD") execLoad(s);
            else if (s.kind == "STORE") execStore(s);
            else throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵，语法免疫系统失效");
        }
    } catch (FclError& e) {
        // 补全行号（若未设置）
        if (e.line < 0) e.line = s.line;
        throw;
    }
    gcTick();
}

// ============================================================
//  指令实现
// ============================================================
void Interp::execIntroduce(Stmt& s) {
    std::string name = s.args[1];  // [INTRODUCE, name, AS, TYPE, WITH, expr...]
    Trophic t = parseTrophic(s.args[3]);
    // 生态圈命名校验：物种在册 + 族群格式 + 营养级匹配
    std::string err = ecoNameError(name, t);
    if (!err.empty()) {
        ErrCode code = (err.rfind("🌿", 0) == 0) ? ErrCode::INVASIVE
                  : (err.rfind("分类学", std::string::npos) != std::string::npos ? ErrCode::TAXONOMY : ErrCode::GENEALOGY);
        throw FclError(code, err);
    }
    // P2-2 修复：重复引种检测（此前静默覆盖，BIOME 段笔误无法发现）
    auto dup = vars_.find(name);
    if (dup != vars_.end()) {
        if (dup->second.type != t)
            throw FclError(ErrCode::TAXONOMY,
                "⚠️ 分类学混乱！" + name + " 已按" + trophicName(dup->second.type) +
                "在册，不可改判为" + trophicName(t));
        if (biomePhase_)
            throw FclError(ErrCode::GENEALOGY,
                "⚠️ 族谱登记混乱！" + name + " 在 BIOME 引种段重复登记");
        // BIOME 之外（循环体内）允许重新引种 = 种群重置（历史惯例，教程依赖）
    }
    size_t wi = 0;
    for (size_t i = 0; i < s.args.size(); i++) if (s.args[i] == "WITH") { wi = i; break; }
    std::string e;
    for (size_t i = wi + 1; i < s.args.size(); i++) e += s.args[i];
    double v = ExprEval::eval(e, mutatedRoots_);
    if (t == PRODUCER && (v < 0 || v > 9999.9)) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵，语法免疫系统失效");
    if (t == APEX && v != 0 && v != 1) v = (v > 0) ? 1 : 0;  // 布尔归一
    if (dup != vars_.end()) {
        // 重置：保留引种顺序与地址，只刷新能量与适应期
        dup->second.value = v;
        dup->second.age = 0;
        dup->second.born = stmtCount_;
        touch(name);
    } else {
        Variable var;
        var.name = name; var.type = t; var.value = v;
        var.age = 0; var.born = stmtCount_; var.alive = true;
        var.id = nextId_++;   // A9：稳定引入序号，不随 GC/EXTINCTION 重排
        vars_[name] = var;
        varOrder_.push_back(name);
        addrMap_[name] = varOrder_.size() - 1;
    }
}

void Interp::execDevour(Stmt& s) {
    std::string pred = s.args[0], prey = s.args[2], algo = s.args[4];
    Variable& p = getVar(pred);
    Variable& q = getVar(prey);
    bool decayer = (s.inDecay && p.type == DECOMPOSER);  // 分解者豁免
    if (!decayer) {
        if (p.type <= q.type || (int)p.type - (int)q.type != 1)
            throw FclError(ErrCode::TROPHIC, "🦴 食性冲突，捕食者拒绝进食");
        // B5 修复：分解者只能在 DECAY 段分解尸体，禁止在 FOODWEB 直接捕食活体
        if (p.type == DECOMPOSER)
            throw FclError(ErrCode::TROPHIC, "🦴 食性冲突，分解者只能在 DECAY 段分解尸体");
    }
    double th = 0;
    if (algo == "SUM") {
        th = p.value + q.value;
    } else if (algo == "DIFF") {
        if (p.value < q.value) throw FclError(ErrCode::STARVE, "🥀 捕食者饿死，能量为负");
        th = p.value - q.value;
    } else if (algo == "PROD" || algo == "QUOT") {
        if (p.type != APEX) throw FclError(ErrCode::TROPHIC, "🦴 食性冲突，只有顶级掠食者可 PROD/QUOT");
        // P2-1 修复：QUOT 除零与表达式除零行为统一（此前静默返回 0）
        if (algo == "QUOT" && q.value == 0)
            throw FclError(ErrCode::DIVZERO, "🔥 干旱导致食物链断裂");
        double raw = (algo == "PROD") ? p.value * q.value : (int)(p.value / q.value);
        // A9 修复：扑咬距离基于变量"稳定 id"（引入序号）的奇偶判定，
        // 不再依赖 varOrder_ 地址表——后者会随 GC / EXTINCTION 重排而改变，
        // 导致同一段 PROD/QUOT 的结果随内存抖动而变。
        if ((p.id + q.id) % 2 == 0) {
            th = raw * 0.5;
            std::cout << "🐾 扑咬落空，能量减半" << std::endl;
        } else {
            th = raw;
        }
    } else {
        throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵，语法免疫系统失效");
    }
    // 能量传递税（分解者豁免；GMO 恢复 100% 效率）
    if (!decayer && !gmo_) th *= 0.2;
    p.value = th;
    // 猎物被消化
    q.value = 0;
    // HERBIVORE 胃溃疡溢出（生态惩罚：真实模式阻塞 2 秒，代码模式即时）
    if (p.type == HERBIVORE && p.value > 255) {
        p.value = 0;
        std::cout << "🤢 胃溃疡溢出，能量归零" << (realMode_ ? "（阻塞 2 秒）" : "（代码模式跳过等待）") << std::endl;
#ifndef FCL_WASM
        if (realMode_) std::this_thread::sleep_for(std::chrono::seconds(2));
#endif
    }
    if (p.type == CARNIVORE) {
        // 奇数行加速 / 偶数行减速 50%（仅真实模式）
#ifndef FCL_WASM
        if (realMode_ && s.line % 2 == 0) std::this_thread::sleep_for(std::chrono::milliseconds(2));
#endif
    }
    touch(pred);
    touch(prey);
}

void Interp::execClone(Stmt& s) {
    // [CLONE, 目标, FROM, 源]：无性繁殖，目标 = 源的值（源不受影响）
    std::string dst = s.args[1], src = s.args[3];
    Variable& sv = getVar(src);
    Variable& dv = getVar(dst);
    dv.value = sv.value;
    touch(dst);
    touch(src);
}

void Interp::requireAPEX(const std::string& name) {
    if (getVar(name).type != APEX)
        throw FclError(ErrCode::TROPHIC, "🦴 食性冲突，只有顶级掠食者（APEX）能承载判定");
}

void Interp::execAssess(Stmt& s) {
    // [ASSESS, A, AGAINST, B, TO, C]：生态位评估，A>=B → C=FULL
    std::string a = s.args[1], b = s.args[3], c = s.args[5];
    Variable& av = getVar(a);
    Variable& bv = getVar(b);
    requireAPEX(c);
    Variable& cv = getVar(c);
    cv.value = (av.value >= bv.value) ? 1.0 : 0.0;
    std::cout << "🧭 生态位评估：" << a << "(" << av.value << ") vs " << b << "(" << bv.value
              << ") → " << c << (cv.value ? " FULL" : " HUNGRY") << std::endl;
    touch(a); touch(b); touch(c);
}

void Interp::execSymbiosis(Stmt& s) {
    // [SYMBIOSIS, A, WITH, B, TO, C]：互利共生，C = A AND B
    std::string a = s.args[1], b = s.args[3], c = s.args[5];
    requireAPEX(a); requireAPEX(b); requireAPEX(c);
    Variable& av = getVar(a);
    Variable& bv = getVar(b);
    Variable& cv = getVar(c);
    cv.value = (av.value != 0 && bv.value != 0) ? 1.0 : 0.0;
    touch(a); touch(b); touch(c);
}

void Interp::execCompetition(Stmt& s) {
    // [COMPETITION, A, OR, B, TO, C]：替代路径，C = A OR B
    std::string a = s.args[1], b = s.args[3], c = s.args[5];
    requireAPEX(a); requireAPEX(b); requireAPEX(c);
    Variable& av = getVar(a);
    Variable& bv = getVar(b);
    Variable& cv = getVar(c);
    cv.value = (av.value != 0 || bv.value != 0) ? 1.0 : 0.0;
    touch(a); touch(b); touch(c);
}

void Interp::execMimicry(Stmt& s) {
    // [MIMICRY, A, TO, B]：拟态伪装，B = NOT A
    std::string a = s.args[1], b = s.args[3];
    requireAPEX(a); requireAPEX(b);
    Variable& av = getVar(a);
    Variable& bv = getVar(b);
    bv.value = (av.value == 0) ? 1.0 : 0.0;
    touch(a); touch(b);
}

void Interp::execRot(Stmt& s) {
    std::string name = s.args[1];
    Variable& v = getVar(name);
    if (v.type != DECOMPOSER) throw FclError(ErrCode::TROPHIC, "🦴 食性冲突，只有分解者可 ROT");
    // v3.0：🧬 转基因标识仅在 REAL MODE 显示（CODE 模式输出保持纯净）
    if (gmo_ && realMode_) std::cout << "🧬";
    if (numericOut_) {
        // 数值输出模式：直接打印数值（如 55 → "55"）
        // 注意：保持与 v3.0.1 一致的截断语义（static_cast），不要用 llround——
        // 否则 PRODUCER 上限 9999.9 会被舍入成 10000，破坏 edge_producer_max 断言。
        std::cout << static_cast<long long>(v.value) << std::flush;
        rotFired_[name] = false;
    } else if (!rotFired_[name]) {
        // 编码模式：第一次输出 ASCII 字符（A7 修复：越界值用 '?' 代替乱码）
        long long iv = std::llround(v.value);
        if (iv < 0 || iv > 255) std::cout << '?';
        else std::cout << (char)iv;
        rotFired_[name] = true;
    } else {
        // 编码模式：第二次输出 Unicode 码位（A7 修复：负数钳到 0，避免 U+FFFFFFFF 之类乱码）
        long long cp = std::llround(v.value);
        if (cp < 0) cp = 0;
        std::printf("U+%04llX", (unsigned long long)cp);
        rotFired_[name] = false;
    }
    std::cout << std::flush;
    touch(name);
}

void Interp::execExtinction(Stmt& s) {
    std::string name = s.args[1];
    // A6 修复：移除"Virus_Crash"死代码分支——该名非生态圈在册物种，永远无法被
    // INTRODUCE，分支不可达；且原实现在函数内调用 std::exit(0)，在 WASM/库场景会
    // 直接杀掉宿主进程，属严重隐患。现统一走正常灭绝逻辑。
    auto it = vars_.find(name);
    if (it == vars_.end()) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵，语法免疫系统失效");
    // 十六进制遗照（16 行 × 16 字节）
    std::cout << "📷 " << name << " 内存遗照：" << std::endl;
    std::hash<std::string> h;
    unsigned long long seed = h(name);
    unsigned char block[256];
    std::memcpy(block, &it->second.value, 8);
    for (int i = 8; i < 256; i++) block[i] = (unsigned char)(seed >> (i % 60));
    for (int r = 0; r < 16; r++) {
        std::printf("%04X  ", r * 16);
        for (int c = 0; c < 16; c++) std::printf("%02X ", block[r * 16 + c]);
        std::cout << "| ";
        for (int c = 0; c < 16; c++) {
            unsigned char b = block[r * 16 + c];
            std::cout << (isprint(b) ? (char)b : '.');
        }
        std::cout << std::endl;
    }
    vars_.erase(it);
    varOrder_.erase(std::remove(varOrder_.begin(), varOrder_.end(), name), varOrder_.end());
    rebuildAddrMap();
}

// ============================================================
//  v3.1 无界存储带（图灵完备核心）
//  设计：稀疏映射 tape_[index]（按需无限增长）+ 可无界移动的带指针 head_。
//  配合算术（DEVOURS / BUMP）、条件分支（ASSESS / SEASON / CASE）与无界
//  循环（WHILE），即 Brainfuck / 图灵机等价 ⇒ 图灵完备。
// ============================================================
long long Interp::curCell() const {
    auto it = tape_.find(head_);
    return (it == tape_.end()) ? 0 : it->second;
}

void Interp::execForward(Stmt& s) { (void)s; head_++; }
void Interp::execBackward(Stmt& s) { (void)s; head_--; }

void Interp::execBump(Stmt& s) {
    // BUMP <expr>：当前带格 += 表达式值（可为负）
    std::string e;
    for (size_t i = 1; i < s.args.size(); i++) {
        if (!e.empty()) e += ' ';
        e += s.args[i];
    }
    if (e.empty()) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：BUMP 缺少增量表达式", s.line);
    double v = ExprEval::eval(e, mutatedRoots_);
    tape_[head_] += (long long)std::llround(v);
}

void Interp::execLoad(Stmt& s) {
    // LOAD <变量>：当前带格 -> 变量
    std::string name = s.args[1];
    Variable& v = getVar(name);
    v.value = (double)curCell();
    touch(name);
}

void Interp::execStore(Stmt& s) {
    // STORE <变量>：变量 -> 当前带格
    std::string name = s.args[1];
    Variable& v = getVar(name);
    tape_[head_] = (long long)std::llround(v.value);
    touch(name);
}

void Interp::execWhile(Stmt& s) {
    // WHILE <物种|TAPE> UNTIL <expr> { ... }
    // 循环条件：物种变量值（或 TAPE = 当前带格）不等于目标值则继续。
    if (s.args.size() < 4 || s.args[2] != "UNTIL")
        throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：WHILE 语法错误", s.line);
    std::string operand = s.args[1];
    std::string exprStr;
    for (size_t i = 3; i < s.args.size(); i++) {
        if (!exprStr.empty()) exprStr += ' ';
        exprStr += s.args[i];
    }
    double target = ExprEval::eval(exprStr, mutatedRoots_);
    const long long MAX_WHILE = 2000000000LL;  // 运行时安全上限（非语言限制）
    long long guard = 0;
    while (true) {
        double cur = (operand == "TAPE") ? (double)curCell() : getVar(operand).value;
        if (cur == target) break;
        if (++guard > MAX_WHILE) {
            std::cout << "⏰ WHILE 超过安全上限，强制退出" << std::endl;
            break;
        }
        execStmts(s.body);
    }
}

// ============================================================
//  v3.0 输入原语（SPROUT 分解）
//  设计哲学：深奥语言不做"高级封装"——输入不再是一条语句，
//  而是嗅探（SCENT）+ 潜伏（LURK）+ 猛扑（POUNCE）三个原子
//  行为的组合。单独一个原语几乎无用，组合起来才能捕获猎物：
//
//    SCENT Wolf_M1 TO Tiger_1 ;         // 嗅探：STDIN 有数据吗？
//    HIBERNATION Wolf_M1 UNTIL Tiger_1 {
//        LURK Wolf_M1 FOR 10 ;          // 潜伏 10 拍再嗅
//        SCENT Wolf_M1 TO Tiger_1 ;
//    }
//    POUNCE Wolf_M1 ;                   // 猛扑：读入数值
// ============================================================
bool Interp::stdinReady() {
#ifdef FCL_WASM
    // 浏览器环境无标准输入：永远嗅不到猎物气味
    return false;
#elif defined(_WIN32)
    HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
    if (h != INVALID_HANDLE_VALUE) {
        DWORD avail = 0;
        if (PeekNamedPipe(h, NULL, 0, NULL, &avail, NULL))
            return avail > 0;  // 管道/重定向输入
        DWORD err = GetLastError();
        if (err == ERROR_BROKEN_PIPE || err == ERROR_NO_DATA)
            return true;  // 管道写端已关闭（EOF）：可读，POUNCE 将判"猎物腐坏"
        DWORD mode = 0;
        if (!GetConsoleMode(h, &mode))
            // 非控制台、非管道（文件 / NUL 等设备重定向）：立即可读，
            // 读到 EOF 的行为由 POUNCE 统一判定为"猎物腐坏"
            return true;
    }
    return _kbhit() != 0;  // 交互控制台
#else
    // POSIX：select 0 超时 = 非阻塞探测（EOF 亦视为可读，与管道断开语义一致）
    fd_set set;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    return select(STDIN_FILENO + 1, &set, nullptr, nullptr, &tv) > 0;
#endif
}

void Interp::execScent(Stmt& s) {
    // [SCENT, 嗅探者, TO, APEX]：非阻塞嗅探风中猎物气味（STDIN 就绪性）
    std::string sniffer = s.args[1], apex = s.args[3];
    requireAPEX(apex);
    Variable& cv = getVar(apex);
    cv.value = stdinReady() ? 1.0 : 0.0;
    std::cout << "👃 " << sniffer << " 嗅探风中气味 → " << apex
              << (cv.value ? " FULL（嗅到猎物）" : " HUNGRY（无气味）") << std::endl;
    touch(apex);
}

void Interp::execLurk(Stmt& s) {
    // [LURK, 物种, FOR, 节拍数]：潜伏等待（不动，保存体力）
    std::string name = s.args[1];
    getVar(name);  // 物种必须在册
    int n = std::atoi(s.args[3].c_str());
    if (n < 0) n = 0;
    if (n > 600) n = 600;  // 上限 600 拍，防止失控等待
    // 节拍时长：REAL 100ms/拍（真实生态节奏）；CODE 1ms/拍（快进视角）
    int ms = realMode_ ? n * 100 : n;
    std::cout << "🕳️ " << name << " 潜伏 " << n << " 拍" << std::endl;
#ifndef FCL_WASM
    if (ms > 0) std::this_thread::sleep_for(std::chrono::milliseconds(ms));
#else
    (void)ms;  // WASM：不等待
#endif
    touch(name);
}

void Interp::execPounce(Stmt& s) {
    // [POUNCE, 捕食者]：非阻塞猛扑。STDIN 有数据 → 读入数值存入该物种；
    // 无数据 → 扑空，能量保持不变（先 SCENT 再 POUNCE 才是正确姿势）
    std::string name = s.args[1];
    Variable& v = getVar(name);
#ifdef FCL_WASM
    std::cout << "⏳ 浏览器版无标准输入，" << name << " 扑空" << std::endl;
    touch(name);
    return;
#endif
    if (!stdinReady()) {
        std::cout << "🐾 " << name << " 扑空（无猎物气味），能量保持" << std::endl;
        touch(name);
        return;
    }
    double x = 0;
    if (std::cin >> x) {
        v.value = x;
        std::cout << "🦅 " << name << " 猛扑命中，捕获能量 " << x << std::endl;
    } else {
        // 流坏损（EOF/非法数值）：丢弃坏数据并视为扑空，避免 HIBERNATION 循环里
        // 反复读同一段坏输入而卡死（C3 修复）
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "🦠 " << name << " 扑到的猎物已腐坏，能量保持" << std::endl;
    }
    touch(name);
}

// ============================================================
//  控制流
// ============================================================
int Interp::countStmts(const std::vector<Stmt>& v) {
    int c = 0;
    for (const auto& s : v) {
        c++;
        if (s.hasBody) c += countStmts(s.body);
    }
    return c;
}

void Interp::execSeason(Stmt& s) {
    bool wet = humidityTrue() >= 2;
    if (s.branch == "RAIN") {
        int cnt = countStmts(s.body);
        // STORM 为显式开关（P0-1 修复）：默认 RAIN 块超过 3 条语句不再乱序
        if (storm_ && cnt > 3) {
            std::cout << "🌩️ RAIN 降级为 STORM，语句随机乱序执行" << std::endl;
            std::shuffle(s.body.begin(), s.body.end(), rng_);
        }
        if (wet) execStmts(s.body);
        else if (s.hasDry) execStmts(s.dryBody);
    } else {
        if (!wet) execStmts(s.body);
    }
}

void Interp::execMigration(Stmt& s) {
    std::string name = s.args[1];  // [MIGRATION, name, OVER, n]
    int n = (s.args.size() > 3) ? std::atoi(s.args[3].c_str()) : 1;
    for (int k = 0; k < n; k++) {
        execStmts(s.body);
        auto it = vars_.find(name);
        if (it != vars_.end()) it->second.value = std::sqrt(it->second.value);
    }
}

void Interp::execHibernation(Stmt& s) {
    // [HIBERNATION, 物种, UNTIL, 条件变量]：冬眠直到条件 FULL 才醒
    if (s.args.size() < 4)
        throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵，语法免疫系统失效", s.line);
    std::string name = s.args[1];
    std::string cond = s.args[3];
    requireAPEX(cond);
    touch(cond);   // A2 修复：条件变量在循环期间被引用，须标记为"活跃"以免被 GC 误回收
    int guard = 0;
    while (getVar(cond).value == 0) {
        if (++guard > 10000) {
            std::cout << "⏰ 冬眠过久（>10000 轮），强制唤醒" << std::endl;
            break;
        }
        execStmts(s.body);
        auto it = vars_.find(name);
        if (it != vars_.end()) it->second.value *= 0.9;  // 冬眠代谢：每轮消耗 10%
        touch(name);
    }
    std::cout << "🌱 " << name << " 冬眠结束，春天来了（" << cond << " FULL）" << std::endl;
}

void Interp::execMutation(Stmt& s) {
    // MUTATION <物种成员名|根名> { CASE "特征": <语句> ... }
    // 语义（与 FCL_SYNTAX 2.9 / FCL_TUTORIAL 第 12 课对齐）：
    //   1/3 概率该物种发生变异（根名 Wolf→Wolv，P0-2：全部 10 物种在册）：
    //   - 物种级改名：块执行期间，该物种全部在册成员临时改名
    //     （Wolf_M1→Wolv_M1、Alpha_Wolf→Alpha_Wolv），块内引用同步改写，
    //     块结束后恢复原名，块外引用不受影响；
    //   - 分支选择：变异触发时随机表达一个 CASE 分支；未触发则整个块空转；
    //   - MATCH() 按物种根名记录变异（Wolf/Wolf_M1/Wolv_M1 均可检测），跨块持续存在。
    if (s.args.size() < 2)
        throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵，语法免疫系统失效", s.line);
    const auto& mut = mutationTable();
    std::string root = canonicalSpecies(speciesRoot(s.args[1]));
    if (root.empty())
        throw FclError(ErrCode::INVASIVE, "🌿 外来物种入侵，生态圈不予接纳！", s.line);
    auto mit = mut.find(root);
    bool fired = (mit != mut.end()) && (rng_() % 3 == 0);

    // 物种级临时改名（块作用域）：先收集成员再改键，避免迭代器失效
    std::vector<std::pair<std::string, std::string>> applied;  // 旧名→新名，块结束后逆序还原
    if (fired) {
        std::cout << "🧬 变异：" << root << " → " << mit->second << std::endl;
        mutatedRoots_.insert(root);
        std::vector<std::string> members;
        for (auto& kv : vars_)
            if (canonicalSpecies(speciesRoot(kv.first)) == root) members.push_back(kv.first);
        for (auto& m : members) {
            std::string nn = renameSpeciesToken(m, root, mit->second);
            if (nn != m) {
                renameVariableInPlace(m, nn);
                applied.emplace_back(m, nn);
                touch(nn);
            }
        }
    }

    // 分支选择：变异触发 → 随机表达一支（无 CASE 标签的块整体作为单一分支）；未触发 → 空转
    if (fired && !s.body.empty()) {
        size_t pick = rng_() % s.body.size();
        std::vector<Stmt> body = s.body[pick].body;  // 拷贝执行：改名不打穿语句树
        renameVar(body, root, mit->second);
        execStmts(body);
    }

    // 恢复临时改名（块外旧名继续有效）
    for (auto it = applied.rbegin(); it != applied.rend(); ++it)
        renameVariableInPlace(it->second, it->first);
}

void Interp::renameVar(std::vector<Stmt>& v, const std::string& from, const std::string& to) {
    // 物种级 token 改写：Wolf_M1 / Alpha_Wolf / Wolf → Wolv_M1 / Alpha_Wolv / Wolv
    for (auto& s : v) {
        for (auto& a : s.args) a = renameSpeciesToken(a, from, to);
        if (s.hasBody) renameVar(s.body, from, to);
    }
}

void Interp::renameVariableInPlace(const std::string& from, const std::string& to) {
    // 在册变量改名：vars_ 键 / varOrder_ / addrMap_ / rotFired_ 同步迁移
    auto it = vars_.find(from);
    if (it == vars_.end()) return;
    Variable v = it->second;
    vars_.erase(it);
    v.name = to;
    vars_[to] = v;
    for (auto& n : varOrder_) if (n == from) n = to;
    auto am = addrMap_.find(from);
    if (am != addrMap_.end()) {
        size_t idx = am->second;
        addrMap_.erase(am);
        addrMap_[to] = idx;
    }
    auto rf = rotFired_.find(from);
    if (rf != rotFired_.end()) {
        bool b = rf->second;
        rotFired_.erase(rf);
        rotFired_[to] = b;
    }
}

// ============================================================
//  垃圾回收（生态分解）
// ============================================================
void Interp::gcTick() {
    if (biomePhase_ || blockDepth_ > 0) return;  // 引种/迁徙等活跃期，分解作用暂停
    std::vector<std::string> toKill;
    for (auto& [k, v] : vars_) {
        if (!v.alive || v.type >= APEX) continue;
        if (stmtCount_ - v.born < 3) continue;  // 适应期：创建后 3 条指令内不回收
        if (touchedThis_.count(k)) continue;    // 本指令被引用，不累计闲置轮数
        v.age++;
        // 只有能量耗尽的"尸体"才被分解；活体（能量>0）不受分解威胁
        if (v.age >= 3 && v.value == 0) toKill.push_back(k);
    }
    bool removed = false;
    for (auto& k : toKill) {
        auto it = vars_.find(k);
        if (it == vars_.end()) continue;
        std::cout << "🍄 " << k << " 被分解者回收" << std::endl;
        // 分解等待仅真实模式生效（模拟微生物分解速度）；代码模式即时回收
#ifndef FCL_WASM
        if (realMode_) {
            int ms = 100 + (int)(rng_() % 901);
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        }
#endif
        vars_.erase(it);
        varOrder_.erase(std::remove(varOrder_.begin(), varOrder_.end(), k), varOrder_.end());
        removed = true;
    }
    if (removed) rebuildAddrMap();
}

} // namespace fcl
