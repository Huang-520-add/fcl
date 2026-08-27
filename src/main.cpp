// ============================================================
//  FCL (FoodChain Language) 解释器 v1.2 参考实现
//  食物链语言：代码在吞噬中传递，真理在分解中显现。
//  C++17 单文件实现 | 编译: g++ -std=c++17 -O2 -o fcl main.cpp
// ============================================================
#include <bits/stdc++.h>
using namespace std;

// ---------- 错误 ----------
struct FclError { string msg; };

// ---------- 营养级 ----------
enum Trophic { PRODUCER = 1, HERBIVORE = 2, CARNIVORE = 3, APEX = 4, DECOMPOSER = 5 };

struct Variable {
    string name;
    Trophic type;
    double value;
    int age = 0;          // 未被引用轮数（GC 用）
    int born = 0;         // 创建时的指令序号（GC 适应期）
    bool alive = true;
};

// ---------- 语句 ----------
struct Stmt {
    bool hasBody = false;
    string kind;              // 简单语句: GMO/INTRODUCE/DEVOURS/ROT/SPROUT/EXTINCTION
    vector<string> args;      // 原始 token 列表（执行器按位置取参）
    string kw;                // 复合块: SEASON/MIGRATION/MUTATION/CASE
    string branch;            // SEASON 的 RAIN/DRY；CASE 的特征
    vector<Stmt> body;
    vector<Stmt> dryBody;     // SEASON 的 DRY 分支（与 RAIN 合并，二选一）
    bool hasDry = false;
    int line = 0;
    bool inDecay = false;     // 是否位于 DECAY 块（分解者豁免）
};

// ---------- 工具 ----------
static string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n"), b = s.find_last_not_of(" \t\r\n");
    return (a == string::npos) ? "" : s.substr(a, b - a + 1);
}

static vector<string> splitWS(const string& s) {
    vector<string> v; string cur;
    bool inStr = false;
    for (char c : s) {
        if (c == '"') { inStr = !inStr; cur += c; }
        else if (isspace((unsigned char)c) && !inStr) { if (!cur.empty()) { v.push_back(cur); cur.clear(); } }
        else cur += c;
    }
    if (!cur.empty()) v.push_back(cur);
    return v;
}

// ============================================================
//  表达式求值（递归下降）：数字 / 分数 / + - * / ( ) / MATCH(name)
// ============================================================
class ExprEval {
    const string& s; size_t pos = 0;
public:
    ExprEval(const string& src) : s(src) {}
    static double eval(const string& src, const map<string, bool>& mutated) {
        ExprEval e(src); e.mut = &mutated; return e.expr();
    }
private:
    const map<string, bool>* mut = nullptr;
    double expr() {
        double v = term();
        while (pos < s.size() && (s[pos] == '+' || s[pos] == '-')) {
            char op = s[pos++]; double r = term();
            v = (op == '+') ? v + r : v - r;
        }
        return v;
    }
    double term() {
        double v = factor();
        while (pos < s.size() && (s[pos] == '*' || s[pos] == '/')) {
            char op = s[pos++]; double r = factor();
            if (op == '*') v = v * r;
            else {
                if (r == 0) throw FclError{"🔥 干旱导致食物链断裂"};
                v = v / r;
            }
        }
        return v;
    }
    double factor() {
        while (pos < s.size() && isspace((unsigned char)s[pos])) pos++;
        if (pos >= s.size()) return 0;
        if (s[pos] == '(') { pos++; double v = expr(); while (pos < s.size() && s[pos] != ')') pos++; pos++; return v; }
        if (s.compare(pos, 5, "MATCH") == 0) {
            pos += 5;
            while (pos < s.size() && s[pos] != '(') pos++;
            pos++; size_t st = pos;
            while (pos < s.size() && s[pos] != ')') pos++;
            string name = trim(s.substr(st, pos - st)); pos++;
            return (mut && mut->count(name)) ? 1.0 : 0.0;
        }
        char* endp = nullptr;
        double v = strtod(s.c_str() + pos, &endp);
        if (endp == s.c_str() + pos) { pos++; return 0; }
        pos = endp - s.c_str();
        return v;
    }
};

// ---------- 生态圈在册物种 ----------
struct EcoSpecies {
    string name;
    Trophic type;
    bool social;  // 群居（有 Alpha 首领 + 性别成员）
};
static const vector<EcoSpecies> ECOLOGY = {
    {"Grass",    PRODUCER,   false},  // 草（种群）
    {"Algae",    PRODUCER,   false},  // 藻（种群）
    {"Sheep",    HERBIVORE,  true},   // 羊（群居，头羊 Alpha_Sheep）
    {"Rabbit",   HERBIVORE,  false},  // 兔（独居）
    {"Wolf",     CARNIVORE,  true},   // 狼（群居，狼王 Alpha_Wolf）
    {"Fox",      CARNIVORE,  false},  // 狐（独居）
    {"Tiger",    APEX,       false},  // 虎（独居）
    {"Lion",     APEX,       true},   // 狮（群居，狮王 Alpha_Lion）
    {"Fungus",   DECOMPOSER, false},  // 真菌（菌落）
    {"Bacillus", DECOMPOSER, false},  // 杆菌（菌落）
};

static string trophicName(Trophic t) {
    switch (t) {
        case PRODUCER: return "生产者";
        case HERBIVORE: return "食草动物";
        case CARNIVORE: return "食肉动物";
        case APEX: return "顶级掠食者";
        case DECOMPOSER: return "分解者";
    }
    return "未知";
}

// 生态圈命名校验：空串=合法，否则为错误信息
static string ecoNameError(const string& name, Trophic type) {
    // Alpha_<物种>：群居首领（如 Alpha_Wolf 狼王）
    if (name.rfind("Alpha_", 0) == 0) {
        string sp = name.substr(6);
        for (auto& e : ECOLOGY) if (e.name == sp) {
            if (!e.social) return "⚠️ 族谱登记混乱！" + sp + " 是独居物种，没有首领";
            if (e.type != type)
                return "⚠️ 分类学混乱！" + sp + " 是" + trophicName(e.type) + "，不是" + trophicName(type);
            return "";
        }
        return "🌿 外来物种入侵，生态圈不予接纳！";
    }
    // <物种>_<标识>
    size_t us = name.find('_');
    if (us == string::npos) return "⚠️ 族谱登记混乱！生态圈物种必须带族群标识";
    string sp = name.substr(0, us);
    string tag = name.substr(us + 1);
    const EcoSpecies* spc = nullptr;
    for (auto& e : ECOLOGY) if (e.name == sp) { spc = &e; break; }
    if (!spc) return "🌿 外来物种入侵，生态圈不予接纳！";
    if (spc->type != type)
        return "⚠️ 分类学混乱！" + sp + " 是" + trophicName(spc->type) + "，不是" + trophicName(type);
    if (spc->social) {
        // 群居：Alpha_物种（首领）或 物种_M/F<编号>（成员，性别标签）
        if (tag == "Alpha") return "";
        if (tag.size() >= 2 && (tag[0] == 'M' || tag[0] == 'F') && isdigit((unsigned char)tag[1])) return "";
        return "⚠️ 族谱登记混乱！群居物种须为 Alpha_<物种> 或 <物种>_<M/F><编号>";
    }
    // 独居 / 种群 / 菌落：物种_<编号>
    bool ok = !tag.empty();
    for (char c : tag) if (!isdigit((unsigned char)c)) { ok = false; break; }
    if (ok) return "";
    return "⚠️ 族谱登记混乱！该物种须为 <物种>_<编号>";
}

// ============================================================
//  解析器：三段式块内容 -> 语句列表
// ============================================================
static vector<Stmt> parseBlock(const string& src, int& lineNo);

static Stmt makeCompound(const string& head, vector<Stmt> body, int lineNo) {
    Stmt s; s.hasBody = true; s.line = lineNo;
    vector<string> t = splitWS(head);
    if (t.empty()) return s;
    string h0 = t[0];
    if (h0 == "SEASON" || h0 == "RAIN" || h0 == "DRY") {
        s.kw = "SEASON";
        s.branch = h0 == "SEASON" ? (t.size() > 1 ? t[1] : "RAIN") : h0;
    } else if (h0 == "MIGRATION") {
        s.kw = "MIGRATION"; s.args = t;
    } else if (h0 == "HIBERNATION") {
        s.kw = "HIBERNATION"; s.args = t;
    } else if (h0 == "MUTATION") {
        s.kw = "MUTATION"; s.args = t;
    } else if (h0 == "CASE") {
        s.kw = "CASE";
        size_t q1 = head.find('"'), q2 = head.find('"', q1 + 1);
        if (q1 != string::npos && q2 != string::npos) s.branch = head.substr(q1 + 1, q2 - q1 - 1);
        else s.branch = "default";
    } else {
        throw FclError{"🌿 变异物种入侵，语法免疫系统失效"};
    }
    s.body = std::move(body);
    return s;
}

static Stmt makeSimple(const string& text, int lineNo) {
    Stmt s; s.line = lineNo;
    vector<string> t = splitWS(text);
    if (t.empty()) throw FclError{"🌿 变异物种入侵，语法免疫系统失效"};
    s.args = t;  // 保留原始 tokens，执行器按位置取参
    if (t[0] == "INTRODUCE") {
        s.kind = "INTRODUCE";
        size_t wi = 0;
        for (size_t i = 0; i < t.size(); i++) if (t[i] == "WITH") { wi = i; break; }
        if (wi == 0 || wi + 1 >= t.size()) throw FclError{"🌿 变异物种入侵，语法免疫系统失效"};
    } else if (t.size() >= 5 && t[1] == "DEVOURS" && t[3] == "USING") {
        s.kind = "DEVOURS";
    } else if (t[0] == "ROT" && t.size() >= 4 && t[2] == "TO") {
        s.kind = "ROT";
    } else if (t[0] == "CLONE" && t.size() >= 4 && t[2] == "FROM") {
        s.kind = "CLONE";
    } else if (t[0] == "ASSESS" && t.size() >= 6 && t[2] == "AGAINST" && t[4] == "TO") {
        s.kind = "ASSESS";
    } else if (t[0] == "SYMBIOSIS" && t.size() >= 6 && t[2] == "WITH" && t[4] == "TO") {
        s.kind = "SYMBIOSIS";
    } else if (t[0] == "COMPETITION" && t.size() >= 6 && t[2] == "OR" && t[4] == "TO") {
        s.kind = "COMPETITION";
    } else if (t[0] == "MIMICRY" && t.size() >= 4 && t[2] == "TO") {
        s.kind = "MIMICRY";
    } else if (t[0] == "SPROUT" && t.size() >= 4 && t[2] == "FROM") {
        s.kind = "SPROUT";
    } else if (t[0] == "EXTINCTION" && t.size() >= 2) {
        s.kind = "EXTINCTION";
    } else if (t[0] == "GMO") {
        s.kind = "GMO";
    } else {
        throw FclError{"🌿 变异物种入侵，语法免疫系统失效"};
    }
    return s;
}

// MUTATION 块内 CASE 分段: "CASE "x": ... " -> vector<pair<特征, 段内容>>
static vector<pair<string, string>> splitCases(const string& src) {
    vector<pair<string, string>> out;
    size_t i = 0, n = src.size();
    while (i < n) {
        size_t p = src.find("CASE", i);
        if (p == string::npos) break;
        size_t q1 = src.find('"', p), q2 = src.find('"', q1 + 1);
        if (q1 == string::npos || q2 == string::npos) break;
        string feat = src.substr(q1 + 1, q2 - q1 - 1);
        size_t colon = src.find(':', q2);
        size_t next = src.find("CASE", colon + 1);
        string seg = (next == string::npos) ? src.substr(colon + 1) : src.substr(colon + 1, next - colon - 1);
        out.push_back({ feat, seg });
        i = (next == string::npos) ? n : next;
    }
    if (out.empty()) out.push_back({ "default", src });
    return out;
}

static vector<Stmt> parseBlock(const string& src, int& lineNo) {
    vector<Stmt> stmts;
    string cur;
    bool inStr = false;
    size_t i = 0, n = src.size();
    while (i < n) {
        char c = src[i];
        if (c == '"') { inStr = !inStr; cur += c; i++; continue; }
        if (inStr) { cur += c; i++; continue; }
        if (c == '\n') { lineNo++; i++; continue; }
        if (c == '{') {
            string head = trim(cur); cur.clear();
            int depth = 1; size_t j = i + 1;
            while (j < n && depth > 0) {
                if (src[j] == '{') depth++;
                else if (src[j] == '}') depth--;
                j++;
            }
            string bodySrc = src.substr(i + 1, j - i - 2);
            int subLine = lineNo;
            string headTrim = trim(head);
            if (headTrim == "DRY") {
                // DRY 分支合并到前一个 SEASON RAIN 块（二选一语义）
                if (stmts.empty() || stmts.back().kw != "SEASON")
                    throw FclError{"🌿 变异物种入侵，语法免疫系统失效"};
                stmts.back().dryBody = parseBlock(bodySrc, subLine);
                stmts.back().hasDry = true;
            } else if (head.rfind("MUTATION", 0) == 0) {
                Stmt s = makeCompound(head, {}, lineNo);
                for (auto& [feat, seg] : splitCases(bodySrc)) {
                    Stmt cs; cs.kw = "CASE"; cs.branch = feat; cs.hasBody = true;
                    cs.body = parseBlock(seg, subLine);
                    cs.line = subLine;
                    s.body.push_back(cs);
                }
                s.hasBody = true;
                stmts.push_back(s);
            } else {
                Stmt s = makeCompound(head, parseBlock(bodySrc, subLine), lineNo);
                stmts.push_back(s);
            }
            i = j;
            continue;
        }
        if (c == ';') {
            string t = trim(cur); cur.clear();
            if (!t.empty()) stmts.push_back(makeSimple(t, lineNo));
            i++;
            continue;
        }
        cur += c; i++;
    }
    string t = trim(cur);
    if (!t.empty()) stmts.push_back(makeSimple(t, lineNo));
    return stmts;
}

// ============================================================
//  解释器
// ============================================================
class Interp {
public:
    void run(const string& raw) {
        string src = stripComments(raw);
        // GMO 检查（首行，三段式之外）
        if (src.rfind("GMO", 0) == 0 || src.find("GMO") < src.find("BIOME")) gmo = true;
        // 三段式提取（按顺序，禁止颠倒）
        string biome = extractBlock(src, "BIOME", 0);
        size_t bEnd = src.find('}', src.find("BIOME"));
        if (bEnd == string::npos) throw FclError{"🌍 生态崩溃，食物链断裂！"};
        string foodweb = extractBlock(src, "FOODWEB", bEnd);
        size_t fEnd = src.find('}', src.find("FOODWEB"));
        if (fEnd == string::npos) throw FclError{"🌍 生态崩溃，食物链断裂！"};
        string decay = extractBlock(src, "DECAY", fEnd);
        // FOODWEB 必须含 DEVOURS
        if (foodweb.find("DEVOURS") == string::npos) throw FclError{"🌍 生态崩溃，食物链断裂！"};

        int line = 1;
        vector<Stmt> b = parseBlock(biome, line);
        vector<Stmt> f = parseBlock(foodweb, line);
        vector<Stmt> d = parseBlock(decay, line);
        for (auto& st : b) st.inDecay = false;
        for (auto& st : f) st.inDecay = false;
        for (auto& st : d) st.inDecay = true;

        biomePhase = true;
        execStmts(b);
        biomePhase = false;
        execStmts(f);
        execStmts(d);
        cout << "\n[FCL] 生态系统运行完毕。现存物种: " << vars.size() << " | 指令总数: " << stmtCount << endl;
    }

private:
    map<string, Variable> vars;
    vector<string> varOrder;
    map<string, bool> mutated;
    map<string, bool> rotFired;
    deque<bool> humidity;      // 最近 3 条指令湿度记录
    set<string> touchedThis;   // 本指令被引用的变量（GC 精确计数）
    int blockDepth = 0;        // 复合控制块嵌套深度（块内暂停 GC）
    bool biomePhase = false;   // BIOME 引种段（创造阶段，不触发分解）
    int stmtCount = 0;
    bool gmo = false;
    mt19937 rng{ random_device{}() };

    // ---------- 预处理 ----------
    string stripComments(const string& raw) {
        stringstream out;
        stringstream in(raw);
        string line;
        regex obsRe(R"(^\s*OBSERVATION:\s*\d{4}-\d{2}-\d{2},\s*Lat:[+-]?[\d.]+,\s*Lon:[+-]?[\d.]+,.*$)");
        while (getline(in, line)) {
            string t = trim(line);
            if (t.empty()) { out << "\n"; continue; }
            if (t.rfind("OBSERVATION:", 0) == 0) {
                if (!regex_match(t, obsRe)) cout << "🔭 缺乏科考精神！" << endl;
                out << "\n";
            } else if (islower((unsigned char)t[0])) {
                out << "\n";  // 小写内容视为注释
            } else {
                out << line << "\n";
            }
        }
        return out.str();
    }

    string extractBlock(const string& src, const string& name, size_t from) {
        size_t p = src.find(name, from);
        if (p == string::npos) throw FclError{"🌍 生态崩溃，食物链断裂！"};
        size_t ob = src.find('{', p);
        if (ob == string::npos) throw FclError{"🌍 生态崩溃，食物链断裂！"};
        int depth = 0;
        for (size_t i = ob; i < src.size(); i++) {
            if (src[i] == '{') depth++;
            else if (src[i] == '}') {
                depth--;
                if (depth == 0) return src.substr(ob + 1, i - ob - 1);
            }
        }
        throw FclError{"🌍 生态崩溃，食物链断裂！"};
    }

    // ---------- 变量 ----------
    Variable& getVar(const string& name) {
        auto it = vars.find(name);
        if (it == vars.end()) throw FclError{"🌿 变异物种入侵，语法免疫系统失效"};
        return it->second;
    }
    void touch(const string& name) {
        auto it = vars.find(name);
        if (it != vars.end()) { it->second.age = 0; touchedThis.insert(name); }
    }
    size_t addrOf(const string& name) {
        for (size_t i = 0; i < varOrder.size(); i++) if (varOrder[i] == name) return i;
        return 0;
    }
    Trophic parseTrophic(const string& s) {
        if (s == "PRODUCER") return PRODUCER;
        if (s == "HERBIVORE") return HERBIVORE;
        if (s == "CARNIVORE") return CARNIVORE;
        if (s == "APEX") return APEX;
        if (s == "DECOMPOSER") return DECOMPOSER;
        throw FclError{"🌿 变异物种入侵，语法免疫系统失效"};
    }

    // ---------- 执行 ----------
    void execStmts(vector<Stmt>& stmts) {
        for (auto& s : stmts) execOne(s);
    }

    void humidityPush(bool isDevour) {
        humidity.push_back(isDevour);
        while (humidity.size() > 3) humidity.pop_front();
    }
    int humidityTrue() { return (int)count(humidity.begin(), humidity.end(), true); }

    void execOne(Stmt& s) {
        stmtCount++;
        touchedThis.clear();
        humidityPush(s.kind == "DEVOURS");
        if (s.hasBody) {
            blockDepth++;
            if (s.kw == "SEASON") execSeason(s);
            else if (s.kw == "MIGRATION") execMigration(s);
            else if (s.kw == "HIBERNATION") execHibernation(s);
            else if (s.kw == "MUTATION") execMutation(s);
            else if (s.kw == "CASE") execStmts(s.body);
            blockDepth--;
        } else {
            if (s.kind == "GMO") { gmo = true; }
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
            else throw FclError{"🌿 变异物种入侵，语法免疫系统失效"};
        }
        gcTick();
    }

    void execIntroduce(Stmt& s) {
        string name = s.args[1];  // [INTRODUCE, name, AS, TYPE, WITH, expr...]
        Trophic t = parseTrophic(s.args[3]);
        // 生态圈命名校验：物种在册 + 族群格式 + 营养级匹配
        string err = ecoNameError(name, t);
        if (!err.empty()) throw FclError{err};
        size_t wi = 0;
        for (size_t i = 0; i < s.args.size(); i++) if (s.args[i] == "WITH") { wi = i; break; }
        string e;
        for (size_t i = wi + 1; i < s.args.size(); i++) e += s.args[i];
        double v = ExprEval::eval(e, mutated);
        if (t == PRODUCER && (v < 0 || v > 9999.9)) throw FclError{"🌿 变异物种入侵，语法免疫系统失效"};
        if (t == APEX && v != 0 && v != 1) v = (v > 0) ? 1 : 0;  // 布尔归一
        vars[name] = Variable{name, t, v, 0, stmtCount, true};
        varOrder.push_back(name);
    }

    void execDevour(Stmt& s) {
        string pred = s.args[0], prey = s.args[2], algo = s.args[4];
        auto& p = getVar(pred);
        auto& q = getVar(prey);
        bool decayer = (s.inDecay && p.type == DECOMPOSER);  // 分解者豁免
        if (!decayer) {
            if (p.type <= q.type || (int)p.type - (int)q.type != 1)
                throw FclError{"🦴 食性冲突，捕食者拒绝进食"};
        }
        double th = 0;
        if (algo == "SUM") th = p.value + q.value;
        else if (algo == "DIFF") {
            if (p.value < q.value) throw FclError{"🥀 捕食者饿死，能量为负"};
            th = p.value - q.value;
        } else if (algo == "PROD" || algo == "QUOT") {
            if (p.type != APEX) throw FclError{"🦴 食性冲突，只有顶级掠食者可 PROD/QUOT"};
            double raw = (algo == "PROD") ? p.value * q.value : (q.value == 0 ? 0 : (int)(p.value / q.value));
            // 扑咬距离：存储地址差为偶数则落空
            size_t di = addrOf(pred), dj = addrOf(prey);
            if ((di + dj) % 2 == 0) { th = raw * 0.5; cout << "🐾 扑咬落空，能量减半" << endl; }
            else th = raw;
        } else {
            throw FclError{"🌿 变异物种入侵，语法免疫系统失效"};
        }
        // 能量传递税（分解者豁免；GMO 恢复 100% 效率）
        if (!decayer && !gmo) th *= 0.2;
        p.value = th;
        // 猎物被消化
        q.value = 0;
        // HERBIVORE 胃溃疡溢出
        if (p.type == HERBIVORE && p.value > 255) {
            p.value = 0;
            cout << "🤢 胃溃疡溢出，能量归零（阻塞 2 秒）" << endl;
            this_thread::sleep_for(chrono::seconds(2));
        }
        if (p.type == CARNIVORE) {
            // 奇数行加速 / 偶数行减速 50%
            if (s.line % 2 == 0) this_thread::sleep_for(chrono::milliseconds(2));
        }
        touch(pred); touch(prey);
    }

    void execClone(Stmt& s) {
        // [CLONE, 目标, FROM, 源]：无性繁殖，目标 = 源的值（源不受影响）
        string dst = s.args[1], src = s.args[3];
        auto& sv = getVar(src);
        auto& dv = getVar(dst);
        dv.value = sv.value;
        touch(dst); touch(src);
    }

    // ---------- v2.0 生态位评估与逻辑 ----------
    // 校验目标必须是 APEX（布尔）
    void requireAPEX(const string& name) {
        if (getVar(name).type != APEX)
            throw FclError{"🦴 食性冲突，只有顶级掠食者（APEX）能承载判定"};
    }
    void execAssess(Stmt& s) {
        // [ASSESS, A, AGAINST, B, TO, C]：生态位评估，A>=B → C=FULL
        string a = s.args[1], b = s.args[3], c = s.args[5];
        auto& av = getVar(a); auto& bv = getVar(b);
        requireAPEX(c);
        auto& cv = getVar(c);
        cv.value = (av.value >= bv.value) ? 1.0 : 0.0;
        cout << "🧭 生态位评估：" << a << "(" << av.value << ") vs " << b << "(" << bv.value
             << ") → " << c << (cv.value ? " FULL" : " HUNGRY") << endl;
        touch(a); touch(b); touch(c);
    }
    void execSymbiosis(Stmt& s) {
        // [SYMBIOSIS, A, WITH, B, TO, C]：互利共生，C = A AND B
        string a = s.args[1], b = s.args[3], c = s.args[5];
        requireAPEX(a); requireAPEX(b); requireAPEX(c);
        auto& av = getVar(a); auto& bv = getVar(b); auto& cv = getVar(c);
        cv.value = (av.value != 0 && bv.value != 0) ? 1.0 : 0.0;
        touch(a); touch(b); touch(c);
    }
    void execCompetition(Stmt& s) {
        // [COMPETITION, A, OR, B, TO, C]：替代路径，C = A OR B
        string a = s.args[1], b = s.args[3], c = s.args[5];
        requireAPEX(a); requireAPEX(b); requireAPEX(c);
        auto& av = getVar(a); auto& bv = getVar(b); auto& cv = getVar(c);
        cv.value = (av.value != 0 || bv.value != 0) ? 1.0 : 0.0;
        touch(a); touch(b); touch(c);
    }
    void execMimicry(Stmt& s) {
        // [MIMICRY, A, TO, B]：拟态伪装，B = NOT A
        string a = s.args[1], b = s.args[3];
        requireAPEX(a); requireAPEX(b);
        auto& av = getVar(a); auto& bv = getVar(b);
        bv.value = (av.value == 0) ? 1.0 : 0.0;
        touch(a); touch(b);
    }

    void execRot(Stmt& s) {
        string name = s.args[1];
        auto& v = getVar(name);
        if (v.type != DECOMPOSER) throw FclError{"🦴 食性冲突，只有分解者可 ROT"};
        if (gmo) cout << "🧬";  // 转基因产品标识
        if (!rotFired[name]) {
            int iv = (int)v.value;
            cout << (char)iv;
            rotFired[name] = true;
        } else {
            printf("U+%04X", (unsigned int)(int)v.value);
            rotFired[name] = false;
        }
        cout << flush;
        touch(name);
    }

    void execSprout(Stmt& s) {
        string name = s.args[1];
        auto& v = getVar(name);
        if (v.type != PRODUCER) throw FclError{"🦴 食性冲突，SPROUT 只能注入生产者"};
        cout << "📡 摩斯电码播放中（2 秒），按空格捕捉数值..." << endl;
        for (int i = 0; i < 10; i++) { cout << '\a' << flush; this_thread::sleep_for(chrono::milliseconds(180)); }
        double num = readNumWithTimeout(2000);
        v.value = num;
        touch(name);
    }

    double readNumWithTimeout(int ms) {
        fd_set set; FD_ZERO(&set); FD_SET(STDIN_FILENO, &set);
        timeval tv; tv.tv_sec = ms / 1000; tv.tv_usec = (ms % 1000) * 1000;
        if (select(STDIN_FILENO + 1, &set, nullptr, nullptr, &tv) > 0) {
            double x; cin >> x; return x;
        }
        cout << "⏰ 捕捉超时，输入为 0" << endl;
        return 0;
    }

    void execExtinction(Stmt& s) {
        string name = s.args[1];
        if (name == "Virus_Crash") { cout << "🦠 Virus_Crash 引爆，进程终止" << endl; exit(0); }
        auto it = vars.find(name);
        if (it == vars.end()) throw FclError{"🌿 变异物种入侵，语法免疫系统失效"};
        // 十六进制遗照（16 行 × 16 字节）
        cout << "📷 " << name << " 内存遗照：" << endl;
        hash<string> h;
        unsigned long long seed = h(name);
        unsigned char block[256];
        memcpy(block, &it->second.value, 8);
        for (int i = 8; i < 256; i++) block[i] = (unsigned char)(seed >> (i % 60));
        for (int r = 0; r < 16; r++) {
            printf("%04X  ", r * 16);
            for (int c = 0; c < 16; c++) printf("%02X ", block[r * 16 + c]);
            cout << "| ";
            for (int c = 0; c < 16; c++) {
                unsigned char b = block[r * 16 + c];
                cout << (isprint(b) ? (char)b : '.');
            }
            cout << endl;
        }
        vars.erase(it);
    }

    // ---------- 控制流 ----------
    static int countStmts(const vector<Stmt>& v) {
        int c = 0;
        for (auto& s : v) { c++; if (s.hasBody) c += countStmts(s.body); }
        return c;
    }
    void execSeason(Stmt& s) {
        bool wet = humidityTrue() >= 2;
        if (s.branch == "RAIN") {
            int cnt = countStmts(s.body);
            if (cnt > 3) {
                cout << "🌩️ RAIN 降级为 STORM，语句随机乱序执行" << endl;
                shuffle(s.body.begin(), s.body.end(), rng);
            }
            if (wet) execStmts(s.body);
            else if (s.hasDry) execStmts(s.dryBody);
        } else {
            if (!wet) execStmts(s.body);
        }
    }
    void execMigration(Stmt& s) {
        string name = s.args[1];  // [MIGRATION, name, OVER, n]
        int n = (s.args.size() > 3) ? atoi(s.args[3].c_str()) : 1;
        for (int k = 0; k < n; k++) {
            execStmts(s.body);
            auto it = vars.find(name);
            if (it != vars.end()) it->second.value = sqrt(it->second.value);
        }
    }
    void execHibernation(Stmt& s) {
        // [HIBERNATION, 物种, UNTIL, 条件变量]：冬眠直到条件 FULL 才醒
        string name = s.args[1];
        string cond = s.args[3];
        requireAPEX(cond);
        int guard = 0;
        while (getVar(cond).value == 0) {
            if (++guard > 10000) { cout << "⏰ 冬眠过久（>10000 轮），强制唤醒" << endl; break; }
            execStmts(s.body);
            auto it = vars.find(name);
            if (it != vars.end()) it->second.value *= 0.9;  // 冬眠代谢：每轮消耗 10%
            touch(name);
        }
        cout << "🌱 " << name << " 冬眠结束，春天来了（" << cond << " FULL）" << endl;
    }
    void execMutation(Stmt& s) {
        string name = s.args[1];
        static map<string, string> mut = { {"Wolf","Wolv"}, {"Sheep","Sheepe"}, {"Tiger","Tygre"}, {"Bacillus","Bacilluz"} };
        string newName = name;
        if (mut.count(name) && (rng() % 3 == 0)) {
            newName = mut[name];
            cout << "🧬 变异：" << name << " → " << newName << endl;
            mutated[name] = true;
        }
        for (auto& cs : s.body) {
            vector<Stmt> body = cs.body;
            renameVar(body, name, newName);
            execStmts(body);
        }
    }
    static void renameVar(vector<Stmt>& v, const string& from, const string& to) {
        for (auto& s : v) {
            for (auto& a : s.args) if (a == from) a = to;
            if (s.hasBody) renameVar(s.body, from, to);
        }
    }

    // ---------- 垃圾回收 ----------
    void gcTick() {
        if (biomePhase || blockDepth > 0) return;  // 引种/迁徙等活跃期，分解作用暂停
        vector<string> toKill;
        for (auto& [k, v] : vars) {
            if (!v.alive || v.type >= APEX) continue;
            if (stmtCount - v.born < 3) continue;  // 适应期：创建后 3 条指令内不回收
            if (touchedThis.count(k)) continue;    // 本指令被引用，不累计闲置轮数
            v.age++;
            // 只有能量耗尽的"尸体"才被分解；活体（能量>0）不受分解威胁
            if (v.age >= 3 && v.value == 0) toKill.push_back(k);
        }
        for (auto& k : toKill) {
            auto it = vars.find(k);
            if (it == vars.end()) continue;
            cout << "🍄 " << k << " 被分解者回收" << endl;
            int ms = 100 + (int)(rng() % 901);
            this_thread::sleep_for(chrono::milliseconds(ms));
            vars.erase(it);
            varOrder.erase(remove(varOrder.begin(), varOrder.end(), k), varOrder.end());
        }
    }
};

// ============================================================
//  main
// ============================================================
int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "用法: fcl <程序文件.fc>" << endl;
        cerr << "FCL 食物链语言 v1.2 —— 代码在吞噬中传递，真理在分解中显现。" << endl;
        return 1;
    }
    ifstream f(argv[1]);
    if (!f) { cerr << "无法打开文件: " << argv[1] << endl; return 1; }
    stringstream buf; buf << f.rdbuf();
    try {
        Interp ip;
        ip.run(buf.str());
    } catch (FclError& e) {
        cout << e.msg << endl;
        return 1;
    }
    return 0;
}