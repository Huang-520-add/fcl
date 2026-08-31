#include "parser.h"
#include "lexer.h"
#include <cctype>

namespace fcl {

std::string trim(const std::string& s) {
    size_t a = s.find_first_not_of(" \t\r\n"), b = s.find_last_not_of(" \t\r\n");
    return (a == std::string::npos) ? "" : s.substr(a, b - a + 1);
}

std::vector<std::string> splitWS(const std::string& s) {
    std::vector<std::string> v;
    std::string cur;
    bool inStr = false;
    for (char c : s) {
        if (c == '"') { inStr = !inStr; cur += c; }
        else if (isspace((unsigned char)c) && !inStr) {
            if (!cur.empty()) { v.push_back(cur); cur.clear(); }
        }
        else cur += c;
    }
    if (!cur.empty()) v.push_back(cur);
    return v;
}

// ------------------------------------------------------------
//  精确 token 校验（P2-3 修复：报错指出期望 token 与实际 token）
// ------------------------------------------------------------
static void expectTok(const std::vector<std::string>& t, size_t idx,
                      const std::string& want, const std::string& shape, int line) {
    std::string actual = (idx < t.size()) ? t[idx] : std::string("<缺失>");
    if (idx >= t.size() || t[idx] != want)
        throw FclError(ErrCode::SYNTAX,
            "🌿 变异物种入侵：应为 " + shape + "，第 " + std::to_string(idx + 1) +
            " 个 token 应为 " + want + "（实际: " + actual + "）", line);
}

static Stmt makeCompound(const std::string& head, std::vector<Stmt> body, int lineNo) {
    Stmt s;
    s.hasBody = true;
    s.line = lineNo;
    std::vector<Token> tk = tokenize(head);
    std::vector<std::string> t;
    for (auto& x : tk) t.push_back(x.text);
    s.toks = std::move(tk);
    if (t.empty()) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：空的控制块头部", lineNo);
    std::string h0 = t[0];
    if (h0 == "SEASON" || h0 == "RAIN" || h0 == "DRY") {
        s.kw = "SEASON";
        s.branch = h0 == "SEASON" ? (t.size() > 1 ? t[1] : "RAIN") : h0;
        if (s.branch != "RAIN" && s.branch != "DRY")
            throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：SEASON 分支应为 RAIN 或 DRY（实际: " + s.branch + "）", lineNo);
    } else if (h0 == "MIGRATION") {
        expectTok(t, 2, "OVER", "MIGRATION <物种> OVER <次数>", lineNo);
        if (t.size() < 4) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：MIGRATION 缺少循环次数", lineNo);
        s.kw = "MIGRATION";
        s.args = t;
    } else if (h0 == "HIBERNATION") {
        expectTok(t, 2, "UNTIL", "HIBERNATION <物种> UNTIL <APEX变量>", lineNo);
        if (t.size() < 4) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：HIBERNATION 缺少条件变量", lineNo);
        s.kw = "HIBERNATION";
        s.args = t;
    } else if (h0 == "MUTATION") {
        if (t.size() < 2) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：MUTATION 缺少物种名", lineNo);
        s.kw = "MUTATION";
        s.args = t;
    } else if (h0 == "CASE") {
        s.kw = "CASE";
        size_t q1 = head.find('"'), q2 = head.find('"', q1 + 1);
        if (q1 != std::string::npos && q2 != std::string::npos) s.branch = head.substr(q1 + 1, q2 - q1 - 1);
        else s.branch = "default";
    } else if (h0 == "WHILE") {
        expectTok(t, 2, "UNTIL", "WHILE <物种|TAPE> UNTIL <expr>", lineNo);
        if (t.size() < 4) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：WHILE 缺少循环条件表达式", lineNo);
        s.kw = "WHILE";
        s.args = t;
    } else {
        throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：未知控制块关键字 " + h0, lineNo);
    }
    s.body = std::move(body);
    return s;
}

static Stmt makeSimple(const std::string& text, int lineNo) {
    Stmt s;
    s.line = lineNo;
    std::vector<Token> tk = tokenize(text);
    std::vector<std::string> t;
    for (auto& x : tk) t.push_back(x.text);
    s.toks = std::move(tk);
    if (t.empty()) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵，语法免疫系统失效", lineNo);
    s.args = t;  // 保留原始 tokens，执行器按位置取参
    if (t[0] == "INTRODUCE") {
        // INTRODUCE <名> AS <营养级> WITH <表达式>
        expectTok(t, 2, "AS", "INTRODUCE <名> AS <营养级> WITH <表达式>", lineNo);
        expectTok(t, 4, "WITH", "INTRODUCE <名> AS <营养级> WITH <表达式>", lineNo);
        s.kind = "INTRODUCE";
    } else if (t.size() > 1 && t[1] == "DEVOURS") {
        // <捕食者> DEVOURS <猎物> USING <算法>
        expectTok(t, 1, "DEVOURS", "<捕食者> DEVOURS <猎物> USING <算法>", lineNo);
        expectTok(t, 3, "USING", "<捕食者> DEVOURS <猎物> USING <算法>", lineNo);
        if (t.size() < 5) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：DEVOURS 缺少算法（SUM/DIFF/PROD/QUOT）", lineNo);
        s.kind = "DEVOURS";
    } else if (t[0] == "CLONE") {
        expectTok(t, 2, "FROM", "CLONE <目标> FROM <源>", lineNo);
        if (t.size() < 4) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：CLONE 缺少源物种", lineNo);
        s.kind = "CLONE";
    } else if (t[0] == "ASSESS") {
        expectTok(t, 2, "AGAINST", "ASSESS <A> AGAINST <B> TO <C>", lineNo);
        expectTok(t, 4, "TO", "ASSESS <A> AGAINST <B> TO <C>", lineNo);
        if (t.size() < 6) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：ASSESS 缺少结果变量", lineNo);
        s.kind = "ASSESS";
    } else if (t[0] == "SYMBIOSIS") {
        expectTok(t, 2, "WITH", "SYMBIOSIS <A> WITH <B> TO <C>", lineNo);
        expectTok(t, 4, "TO", "SYMBIOSIS <A> WITH <B> TO <C>", lineNo);
        if (t.size() < 6) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：SYMBIOSIS 缺少结果变量", lineNo);
        s.kind = "SYMBIOSIS";
    } else if (t[0] == "COMPETITION") {
        expectTok(t, 2, "OR", "COMPETITION <A> OR <B> TO <C>", lineNo);
        expectTok(t, 4, "TO", "COMPETITION <A> OR <B> TO <C>", lineNo);
        if (t.size() < 6) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：COMPETITION 缺少结果变量", lineNo);
        s.kind = "COMPETITION";
    } else if (t[0] == "MIMICRY") {
        expectTok(t, 2, "TO", "MIMICRY <A> TO <B>", lineNo);
        if (t.size() < 4) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：MIMICRY 缺少目标", lineNo);
        s.kind = "MIMICRY";
    } else if (t[0] == "ROT") {
        expectTok(t, 2, "TO", "ROT <分解者> TO STDOUT", lineNo);
        s.kind = "ROT";
    } else if (t[0] == "SCENT") {
        // v3.0 新原语：嗅探（非阻塞检测 STDIN 是否有猎物气味）
        expectTok(t, 2, "TO", "SCENT <嗅探者> TO <APEX>", lineNo);
        if (t.size() < 4) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：SCENT 缺少 APEX 结果变量", lineNo);
        s.kind = "SCENT";
    } else if (t[0] == "LURK") {
        // v3.0 新原语：潜伏等待 n 拍
        expectTok(t, 2, "FOR", "LURK <物种> FOR <节拍数>", lineNo);
        if (t.size() < 4) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：LURK 缺少节拍数", lineNo);
        s.kind = "LURK";
    } else if (t[0] == "POUNCE") {
        // v3.0 新原语：非阻塞猛扑（STDIN 有数据则读入，否则扑空）
        if (t.size() < 2) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：POUNCE 缺少物种名", lineNo);
        s.kind = "POUNCE";
    } else if (t[0] == "EXTINCTION") {
        if (t.size() < 2) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：EXTINCTION 缺少物种名", lineNo);
        s.kind = "EXTINCTION";
    } else if (t[0] == "GMO") {
        expectTok(t, 1, "ENABLED", "GMO ENABLED", lineNo);
        s.kind = "GMO";
    } else if (t[0] == "STORM") {
        expectTok(t, 1, "ENABLED", "STORM ENABLED", lineNo);
        s.kind = "STORM";
    } else if (t[0] == "NUMERIC" && t.size() >= 2 && t[1] == "OUTPUT") {
        s.kind = "NUMERIC";
    } else if (t[0] == "REAL" && t.size() >= 2 && t[1] == "MODE") {
        s.kind = "REALMODE";
    } else if (t[0] == "CODE" && t.size() >= 2 && t[1] == "MODE") {
        s.kind = "CODEMODE";
    } else if (t[0] == "FORWARD") {
        s.kind = "FORWARD";
    } else if (t[0] == "BACKWARD") {
        s.kind = "BACKWARD";
    } else if (t[0] == "BUMP") {
        if (t.size() < 2) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：BUMP 缺少增量表达式", lineNo);
        s.kind = "BUMP";
    } else if (t[0] == "LOAD") {
        if (t.size() < 2) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：LOAD 缺少目标变量", lineNo);
        s.kind = "LOAD";
    } else if (t[0] == "STORE") {
        if (t.size() < 2) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：STORE 缺少源变量", lineNo);
        s.kind = "STORE";
    } else {
        throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：未知语句关键字 " + t[0], lineNo);
    }
    return s;
}

// ------------------------------------------------------------
//  MUTATION 块内 CASE 分段
//  P2-5 修复：字符串感知，"..." 内的 CASE 不切分
//  P3-1 修复：花括号深度感知——嵌套块（CASE 段内再套 MUTATION { CASE ... } 等）
//            内层的 CASE 属于内层作用域，外层 splitCases 不得切分
// ------------------------------------------------------------
static size_t findCaseOutsideStrings(const std::string& src, size_t from) {
    bool inStr = false;
    int depth = 0;
    for (size_t i = from; i < src.size(); i++) {
        char c = src[i];
        if (c == '"') { inStr = !inStr; continue; }
        if (inStr) continue;
        if (c == '{') { depth++; continue; }
        if (c == '}') { if (depth > 0) depth--; continue; }
        if (depth == 0 && src.compare(i, 4, "CASE") == 0) return i;
    }
    return std::string::npos;
}

static std::vector<std::pair<std::string, std::string>> splitCases(const std::string& src) {
    std::vector<std::pair<std::string, std::string>> out;
    size_t i = 0, n = src.size();
    while (true) {
        size_t p = findCaseOutsideStrings(src, i);
        if (p == std::string::npos) break;
        size_t q1 = src.find('"', p), q2 = src.find('"', q1 + 1);
        if (q1 == std::string::npos || q2 == std::string::npos)
            throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：CASE 标签缺少成对引号");
        std::string feat = src.substr(q1 + 1, q2 - q1 - 1);
        size_t colon = src.find(':', q2);
        if (colon == std::string::npos)
            throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：CASE 标签后缺少冒号");
        size_t next = findCaseOutsideStrings(src, colon + 1);
        std::string seg = (next == std::string::npos) ? src.substr(colon + 1) : src.substr(colon + 1, next - colon - 1);
        out.push_back({ feat, seg });
        i = (next == std::string::npos) ? n : next;
        if (next == std::string::npos) break;
    }
    if (out.empty()) out.push_back({ "default", src });
    return out;
}

std::vector<Stmt> parseBlock(const std::string& src, int& lineNo, int depth) {
    // 嵌套深度守卫（P2-7 修复：防止递归下降栈溢出）
    if (depth > MAX_NEST_DEPTH)
        throw FclError(ErrCode::SYNTAX,
            "🌿 变异物种入侵：控制块嵌套超过 " + std::to_string(MAX_NEST_DEPTH) + " 层", lineNo);
    std::vector<Stmt> stmts;
    std::string cur;
    bool inStr = false;
    size_t i = 0, n = src.size();
    while (i < n) {
        char c = src[i];
        if (c == '"') { inStr = !inStr; cur += c; i++; continue; }
        if (inStr) { cur += c; i++; continue; }
        if (c == '\n') { lineNo++; i++; continue; }
        if (c == '{') {
            std::string head = trim(cur);
            cur.clear();
            int braceDepth = 1;
            bool bodyStr = false;  // 字符串内的 { } 不计入块深度
            size_t j = i + 1;
            while (j < n && braceDepth > 0) {
                if (src[j] == '"') { bodyStr = !bodyStr; j++; continue; }
                if (!bodyStr) {
                    if (src[j] == '{') braceDepth++;
                    else if (src[j] == '}') braceDepth--;
                }
                j++;
            }
            std::string bodySrc = src.substr(i + 1, j - i - 2);
            int subLine = lineNo;
            std::string headTrim = trim(head);
            if (headTrim == "DRY") {
                // DRY 分支合并到前一个 SEASON RAIN 块（二选一语义）
                if (stmts.empty() || stmts.back().kw != "SEASON")
                    throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵：DRY 分支必须紧跟 SEASON RAIN 块", lineNo);
                stmts.back().dryBody = parseBlock(bodySrc, subLine, depth + 1);
                stmts.back().hasDry = true;
            } else if (head.rfind("MUTATION", 0) == 0) {
                Stmt s = makeCompound(head, {}, lineNo);
                for (auto& [feat, seg] : splitCases(bodySrc)) {
                    Stmt cs;
                    cs.kw = "CASE";
                    cs.branch = feat;
                    cs.hasBody = true;
                    cs.body = parseBlock(seg, subLine, depth + 1);
                    cs.line = subLine;
                    s.body.push_back(cs);
                }
                s.hasBody = true;
                stmts.push_back(s);
            } else {
                Stmt s = makeCompound(head, parseBlock(bodySrc, subLine, depth + 1), lineNo);
                stmts.push_back(s);
            }
            i = j;
            continue;
        }
        if (c == ';') {
            std::string t = trim(cur);
            cur.clear();
            if (!t.empty()) stmts.push_back(makeSimple(t, lineNo));
            i++;
            continue;
        }
        cur += c;
        i++;
    }
    std::string t = trim(cur);
    if (!t.empty()) stmts.push_back(makeSimple(t, lineNo));
    return stmts;
}

} // namespace fcl
