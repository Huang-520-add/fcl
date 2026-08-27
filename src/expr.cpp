#include "expr.h"
#include <cstdlib>
#include <cctype>

namespace fcl {

double ExprEval::eval(const std::string& src, const std::map<std::string, bool>& mutated) {
    ExprEval e(src);
    e.mut_ = &mutated;
    double v = e.expr();
    // 表达式解析后必须到达末尾，残留 token = 语法错误（P1-4 修复）
    while (e.pos_ < e.s_.size() && isspace((unsigned char)e.s_[e.pos_])) e.pos_++;
    if (e.pos_ != e.s_.size())
        throw Error(Err::EXPR, "🌿 变异物种入侵，表达式含无法解析的内容");
    return v;
}

double ExprEval::expr() {
    double v = term();
    while (pos_ < s_.size() && (s_[pos_] == '+' || s_[pos_] == '-')) {
        char op = s_[pos_++];
        double r = term();
        v = (op == '+') ? v + r : v - r;
    }
    return v;
}

double ExprEval::term() {
    double v = factor();
    while (pos_ < s_.size() && (s_[pos_] == '*' || s_[pos_] == '/')) {
        char op = s_[pos_++];
        double r = factor();
        if (op == '*') {
            v = v * r;
        } else {
            if (r == 0) throw Error(Err::DIVZERO, "🔥 干旱导致食物链断裂");
            v = v / r;
        }
    }
    return v;
}

double ExprEval::factor() {
    while (pos_ < s_.size() && isspace((unsigned char)s_[pos_])) pos_++;
    if (pos_ >= s_.size()) return 0;
    if (s_[pos_] == '(') {
        pos_++;
        double v = expr();
        while (pos_ < s_.size() && s_[pos_] != ')') pos_++;
        pos_++;
        return v;
    }
    if (s_.compare(pos_, 5, "MATCH") == 0) {
        pos_ += 5;
        while (pos_ < s_.size() && s_[pos_] != '(') pos_++;
        pos_++;
        size_t st = pos_;
        while (pos_ < s_.size() && s_[pos_] != ')') pos_++;
        std::string name = s_.substr(st, pos_ - st);
        // 去掉首尾空白
        size_t a = name.find_first_not_of(" \t"), b = name.find_last_not_of(" \t");
        name = (a == std::string::npos) ? "" : name.substr(a, b - a + 1);
        pos_++;
        return (mut_ && mut_->count(name)) ? 1.0 : 0.0;
    }
    // 数字解析：失败即抛错（P1-4 修复，不再静默返回 0）
    char* endp = nullptr;
    double v = strtod(s_.c_str() + pos_, &endp);
    if (endp == s_.c_str() + pos_)
        throw Error(Err::EXPR, "🌿 变异物种入侵，表达式含无法解析的内容");
    pos_ = endp - s_.c_str();
    return v;
}

} // namespace fcl
