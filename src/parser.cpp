#include "parser.h"
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

static Stmt makeCompound(const std::string& head, std::vector<Stmt> body, int lineNo) {
    Stmt s;
    s.hasBody = true;
    s.line = lineNo;
    std::vector<std::string> t = splitWS(head);
    if (t.empty()) return s;
    std::string h0 = t[0];
    if (h0 == "SEASON" || h0 == "RAIN" || h0 == "DRY") {
        s.kw = "SEASON";
        s.branch = h0 == "SEASON" ? (t.size() > 1 ? t[1] : "RAIN") : h0;
    } else if (h0 == "MIGRATION") {
        s.kw = "MIGRATION";
        s.args = t;
    } else if (h0 == "HIBERNATION") {
        s.kw = "HIBERNATION";
        s.args = t;
    } else if (h0 == "MUTATION") {
        s.kw = "MUTATION";
        s.args = t;
    } else if (h0 == "CASE") {
        s.kw = "CASE";
        size_t q1 = head.find('"'), q2 = head.find('"', q1 + 1);
        if (q1 != std::string::npos && q2 != std::string::npos) s.branch = head.substr(q1 + 1, q2 - q1 - 1);
        else s.branch = "default";
    } else {
        throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵，语法免疫系统失效", lineNo);
    }
    s.body = std::move(body);
    return s;
}

static Stmt makeSimple(const std::string& text, int lineNo) {
    Stmt s;
    s.line = lineNo;
    std::vector<std::string> t = splitWS(text);
    if (t.empty()) throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵，语法免疫系统失效", lineNo);
    s.args = t;  // 保留原始 tokens，执行器按位置取参
    if (t[0] == "INTRODUCE") {
        s.kind = "INTRODUCE";
        size_t wi = 0;
        for (size_t i = 0; i < t.size(); i++) if (t[i] == "WITH") { wi = i; break; }
        if (wi == 0 || wi + 1 >= t.size())
            throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵，语法免疫系统失效", lineNo);
    } else if (t.size() >= 5 && t[1] == "DEVOURS" && t[3] == "USING") {
        s.kind = "DEVOURS";
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
    } else if (t[0] == "ROT" && t.size() >= 4 && t[2] == "TO") {
        s.kind = "ROT";
    } else if (t[0] == "SPROUT" && t.size() >= 4 && t[2] == "FROM") {
        s.kind = "SPROUT";
    } else if (t[0] == "EXTINCTION" && t.size() >= 2) {
        s.kind = "EXTINCTION";
    } else if (t[0] == "GMO") {
        s.kind = "GMO";
    } else if (t[0] == "STORM") {
        s.kind = "STORM";
    } else if (t[0] == "NUMERIC" && t.size() >= 2 && t[1] == "OUTPUT") {
        s.kind = "NUMERIC";
    } else if (t[0] == "REAL" && t.size() >= 2 && t[1] == "MODE") {
        s.kind = "REALMODE";
    } else if (t[0] == "CODE" && t.size() >= 2 && t[1] == "MODE") {
        s.kind = "CODEMODE";
    } else {
        throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵，语法免疫系统失效", lineNo);
    }
    return s;
}

// MUTATION 块内 CASE 分段: "CASE "x": ... " -> vector<pair<特征, 段内容>>
static std::vector<std::pair<std::string, std::string>> splitCases(const std::string& src) {
    std::vector<std::pair<std::string, std::string>> out;
    size_t i = 0, n = src.size();
    while (i < n) {
        size_t p = src.find("CASE", i);
        if (p == std::string::npos) break;
        size_t q1 = src.find('"', p), q2 = src.find('"', q1 + 1);
        if (q1 == std::string::npos || q2 == std::string::npos) break;
        std::string feat = src.substr(q1 + 1, q2 - q1 - 1);
        size_t colon = src.find(':', q2);
        size_t next = src.find("CASE", colon + 1);
        std::string seg = (next == std::string::npos) ? src.substr(colon + 1) : src.substr(colon + 1, next - colon - 1);
        out.push_back({ feat, seg });
        i = (next == std::string::npos) ? n : next;
    }
    if (out.empty()) out.push_back({ "default", src });
    return out;
}

std::vector<Stmt> parseBlock(const std::string& src, int& lineNo) {
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
            int depth = 1;
            size_t j = i + 1;
            while (j < n && depth > 0) {
                if (src[j] == '{') depth++;
                else if (src[j] == '}') depth--;
                j++;
            }
            std::string bodySrc = src.substr(i + 1, j - i - 2);
            int subLine = lineNo;
            std::string headTrim = trim(head);
            if (headTrim == "DRY") {
                // DRY 分支合并到前一个 SEASON RAIN 块（二选一语义）
                if (stmts.empty() || stmts.back().kw != "SEASON")
                    throw FclError(ErrCode::SYNTAX, "🌿 变异物种入侵，语法免疫系统失效", lineNo);
                stmts.back().dryBody = parseBlock(bodySrc, subLine);
                stmts.back().hasDry = true;
            } else if (head.rfind("MUTATION", 0) == 0) {
                Stmt s = makeCompound(head, {}, lineNo);
                for (auto& [feat, seg] : splitCases(bodySrc)) {
                    Stmt cs;
                    cs.kw = "CASE";
                    cs.branch = feat;
                    cs.hasBody = true;
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
