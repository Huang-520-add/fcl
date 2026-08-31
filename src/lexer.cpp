#include "lexer.h"
#include <cctype>

namespace fcl {
namespace {

bool isKeyword(const std::string& w) {
    static const char* KW[] = {
        "INTRODUCE","DEVOURS","CLONE","ASSESS","SYMBIOSIS","COMPETITION",
        "MIMICRY","ROT","SCENT","LURK","POUNCE","EXTINCTION","SEASON",
        "MIGRATION","HIBERNATION","MUTATION","CASE","GMO","STORM","NUMERIC",
        "REAL","CODE","BIOME","FOODWEB","DECAY","FORWARD","BACKWARD","BUMP",
        "LOAD","STORE","WHILE","UNTIL","AS","WITH","USING","FROM","AGAINST",
        "TO","OR","FOR","ENABLED","MODE","OUTPUT","OVER","RAIN","DRY", nullptr};
    for (const char** k = KW; *k; ++k) if (w == *k) return true;
    return false;
}

TokType classify(const std::string& t) {
    if (!t.empty() && t.front() == '"') return TokType::STR;
    if (isKeyword(t)) return TokType::KW;
    // 数字字面量（允许一个前导负号与一个小数点）
    bool num = true, seenDot = false;
    for (size_t i = 0; i < t.size(); ++i) {
        char c = t[i];
        if (i == 0 && c == '-') continue;
        if (c == '.') { if (seenDot) { num = false; break; } seenDot = true; continue; }
        if (!std::isdigit((unsigned char)c)) { num = false; break; }
    }
    if (num && t.size() > 0 && !(t.size() == 1 && t[0] == '-')) return TokType::NUM;
    return TokType::IDENT;
}

} // namespace

// ------------------------------------------------------------
//  语句级分词：与历史 splitWS 行为一致
// ------------------------------------------------------------
std::vector<Token> tokenize(const std::string& s) {
    std::vector<Token> v;
    std::string cur;
    bool inStr = false;
    for (char c : s) {
        if (c == '"') { inStr = !inStr; cur += c; }
        else if (std::isspace((unsigned char)c) && !inStr) {
            if (!cur.empty()) { v.push_back({ classify(cur), cur, 0 }); cur.clear(); }
        } else cur += c;
    }
    if (!cur.empty()) v.push_back({ classify(cur), cur, 0 });
    return v;
}

// ------------------------------------------------------------
//  全量结构化分词：识别块 / 分隔 / 运算符
// ------------------------------------------------------------
std::vector<Token> tokenizeFull(const std::string& src) {
    std::vector<Token> v;
    std::string cur;
    bool inStr = false;
    int line = 1;
    auto flush = [&]() {
        if (!cur.empty()) { v.push_back({ classify(cur), cur, line }); cur.clear(); }
    };
    for (size_t i = 0; i < src.size(); ++i) {
        char c = src[i];
        if (c == '\n') { flush(); line++; continue; }
        if (c == '"') { inStr = !inStr; cur += c; continue; }
        if (inStr) { cur += c; continue; }
        if (c == '{' || c == '}' || c == ';' || c == ':') {
            flush();
            TokType ty = (c == '{') ? TokType::LBRACE : (c == '}') ? TokType::RBRACE
                       : (c == ';') ? TokType::SEMI : TokType::COLON;
            v.push_back({ ty, std::string(1, c), line });
            continue;
        }
        if (c == '(' || c == ')' || c == '+' || c == '-' || c == '*' || c == '/' || c == '=') {
            flush();
            v.push_back({ TokType::OP, std::string(1, c), line });
            continue;
        }
        if (std::isspace((unsigned char)c)) { flush(); continue; }
        cur += c;
    }
    flush();
    return v;
}

} // namespace fcl
