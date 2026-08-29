#include "interpreter.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <regex>
#include <thread>

#ifndef _WIN32
#include <sys/select.h>
#include <unistd.h>
#else
#include <conio.h>
#include <windows.h>
#endif

namespace fcl {

// ============================================================
//  运行入口
// ============================================================
void Interp::run(const std::string& raw) {
    std::string src = stripComments(raw);
    // 全局开关（三段式之外，程序首行）：GMO / STORM / NUMERIC OUTPUT / REAL MODE
    size_t biomePos = src.find("BIOME");
    if (src.find("GMO") < biomePos) gmo_ = true;
    if (src.find("STORM") < biomePos) storm_ = true;
    if (src.find("NUMERIC") < biomePos) numericOut_ = true;
    if (src.find("REAL MODE") < biomePos) realMode_ = true;

    // 三段式提取（按顺序，禁止颠倒）
    std::string biome = extractBlock(src, "BIOME", 0);
    size_t bEnd = src.find('}', src.find("BIOME"));
    if (bEnd == std::string::npos) throw FclError(ErrCode::STRUCTURE, "🌍 生态崩溃，食物链断裂！");
    std::string foodweb = extractBlock(src, "FOODWEB", bEnd);
    size_t fEnd = src.find('}', src.find("FOODWEB"));
    if (fEnd == std::string::npos) throw FclError(ErrCode::STRUCTURE, "🌍 生态崩溃，食物链断裂！");
    std::string decay = extractBlock(src, "DECAY", fEnd);
    // FOODWEB 必须含 DEVOURS
    if (foodweb.find("DEVOURS") == std::string::npos)
        throw FclError(ErrCode::STRUCTURE, "🌍 生态崩溃，食物链断裂！");

    int line = 1;
    std::vector<Stmt> b = parseBlock(biome, line);
    std::vector<Stmt> f = parseBlock(foodweb, line);
    std::vector<Stmt> d = parseBlock(decay, line);
    for (auto& st : b) st.inDecay = false;
    for (auto& st : f) st.inDecay = false;
    for (auto& st : d) st.inDecay = true;

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
    for (size_t i = 0; i < varOrder_.size(); i++) if (varOrder_[i] == name) return i;
    return 0;
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
            else if (s.kind == "SPROUT") execSprout(s);
            else if (s.kind == "EXTINCTION") execExtinction(s);
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
    size_t wi = 0;
    for (size_t i = 0; i < s.args.size(); i++) if (s.args[i] == "WITH") { wi = i; break; }
    std::string e;
    for (size_t i = wi + 1; i < s.args.size(); i++) e += s.args[i];
    double v = ExprEval::eval(e, mutatedRoots_);
    if (t == PRODUCER && (v < 0 || v > 9999.9)) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵，语法免疫系统失效");
    if (t == APEX && v != 0 && v != 1) v = (v > 0) ? 1 : 0;  // 布尔归一
    vars_[name] = Variable{ name, t, v, 0, stmtCount_, true };
    varOrder_.push_back(name);
}

void Interp::execDevour(Stmt& s) {
    std::string pred = s.args[0], prey = s.args[2], algo = s.args[4];
    Variable& p = getVar(pred);
    Variable& q = getVar(prey);
    bool decayer = (s.inDecay && p.type == DECOMPOSER);  // 分解者豁免
    if (!decayer) {
        if (p.type <= q.type || (int)p.type - (int)q.type != 1)
            throw FclError(ErrCode::TROPHIC, "🦴 食性冲突，捕食者拒绝进食");
    }
    double th = 0;
    if (algo == "SUM") {
        th = p.value + q.value;
    } else if (algo == "DIFF") {
        if (p.value < q.value) throw FclError(ErrCode::STARVE, "🥀 捕食者饿死，能量为负");
        th = p.value - q.value;
    } else if (algo == "PROD" || algo == "QUOT") {
        if (p.type != APEX) throw FclError(ErrCode::TROPHIC, "🦴 食性冲突，只有顶级掠食者可 PROD/QUOT");
        double raw = (algo == "PROD") ? p.value * q.value : (q.value == 0 ? 0 : (int)(p.value / q.value));
        // 扑咬距离：存储地址差为偶数则落空
        size_t di = addrOf(pred), dj = addrOf(prey);
        if ((di + dj) % 2 == 0) {
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
    if (gmo_) std::cout << "🧬";  // 转基因产品标识
    if (numericOut_) {
        // 数值输出模式：直接打印数值（如 55 → "55"）
        std::cout << static_cast<long long>(v.value) << std::flush;
        rotFired_[name] = false;
    } else if (!rotFired_[name]) {
        // 编码模式：第一次输出 ASCII 字符
        int iv = (int)v.value;
        std::cout << (char)iv;
        rotFired_[name] = true;
    } else {
        // 编码模式：第二次输出 Unicode 码位
        std::printf("U+%04X", (unsigned int)(int)v.value);
        rotFired_[name] = false;
    }
    std::cout << std::flush;
    touch(name);
}

void Interp::execSprout(Stmt& s) {
    std::string name = s.args[1];
    Variable& v = getVar(name);
    if (v.type != PRODUCER) throw FclError(ErrCode::TROPHIC, "🦴 食性冲突，SPROUT 只能注入生产者");
#ifndef FCL_WASM
    // 摩斯电码铃声（终端 10 声蜂鸣，约 2 秒）；浏览器版（WASM）跳过等待，避免阻塞 UI
    std::cout << "📡 摩斯电码播放中（2 秒），按空格捕捉数值..." << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << '\a' << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(180));
    }
#endif
    double num = readNumWithTimeout(2000);
    v.value = num;
    touch(name);
}

double Interp::readNumWithTimeout(int ms) {
#ifdef FCL_WASM
    // WASM/浏览器环境无标准输入：SPROUT 直接返回 0（P1-5 修复）
    std::cout << "⏰ 浏览器版不支持标准输入，SPROUT 返回 0" << std::endl;
    return 0;
#elif defined(_WIN32)
    // Windows：真超时（P0-3 修复）
    // 管道输入（重定向/CI）：PeekNamedPipe 检测数据，无数据立即返回 0
    HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
    DWORD avail = 0;
    if (h != INVALID_HANDLE_VALUE && PeekNamedPipe(h, NULL, 0, NULL, &avail, NULL)) {
        if (avail > 0) {
            double x;
            std::cin >> x;
            return x;
        }
        std::cout << "⏰ 捕捉超时，输入为 0" << std::endl;
        return 0;
    }
    // 控制台交互：_kbhit 轮询实现超时
    for (int i = 0; i < ms / 50; i++) {
        if (_kbhit()) {
            double x;
            std::cin >> x;
            return x;
        }
        Sleep(50);
    }
    std::cout << "⏰ 捕捉超时，输入为 0" << std::endl;
    return 0;
#else
    // POSIX：select 2000ms 超时
    fd_set set;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    timeval tv;
    tv.tv_sec = ms / 1000;
    tv.tv_usec = (ms % 1000) * 1000;
    if (select(STDIN_FILENO + 1, &set, nullptr, nullptr, &tv) > 0) {
        double x;
        std::cin >> x;
        return x;
    }
    std::cout << "⏰ 捕捉超时，输入为 0" << std::endl;
    return 0;
#endif
}

void Interp::execExtinction(Stmt& s) {
    std::string name = s.args[1];
    if (name == "Virus_Crash") {
        std::cout << "🦠 Virus_Crash 引爆，进程终止" << std::endl;
        std::exit(0);
    }
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
    // 在册变量改名：vars_ 键 / varOrder_ / rotFired_ 同步迁移
    auto it = vars_.find(from);
    if (it == vars_.end()) return;
    Variable v = it->second;
    vars_.erase(it);
    v.name = to;
    vars_[to] = v;
    for (auto& n : varOrder_) if (n == from) n = to;
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
    }
}

} // namespace fcl
