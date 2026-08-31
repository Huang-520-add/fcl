#include "expr.h"
#include "ecology.h"
#include <cstdlib>
#include <cctype>

namespace fcl {

double ExprEval::eval(const std::string& src, const std::set<std::string>& mutatedRoots) {
    ExprEval e(src);
    e.mut_ = &mutatedRoots;
    double v = e.expr();
    // 表达式解析后必须到达末尾，残留 token = 语法错误（P1-4 修复）
    while (e.pos_ < e.s_.size() && isspace((unsigned char)e.s_[e.pos_])) e.pos_++;
    if (e.pos_ != e.s_.size())
        throw FclError(ErrCode::EXPR, "🌿 变异物种入侵，表达式含无法解析的内容");
    return v;
}

double ExprEval::expr() {
    double v = term();
    while (true) {
        while (pos_ < s_.size() && isspace((unsigned char)s_[pos_])) pos_++;
        if (pos_ < s_.size() && (s_[pos_] == '+' || s_[pos_] == '-')) {
            char op = s_[pos_++];
            double r = term();
            v = (op == '+') ? v + r : v - r;
        } else break;
    }
    return v;
}

double ExprEval::term() {
    double v = factor();
    while (true) {
        while (pos_ < s_.size() && isspace((unsigned char)s_[pos_])) pos_++;
        if (pos_ < s_.size() && (s_[pos_] == '*' || s_[pos_] == '/')) {
            char op = s_[pos_++];
            double r = factor();
            if (op == '*') {
                v = v * r;
            } else {
                if (r == 0) throw FclError(ErrCode::DIVZERO, "🔥 干旱导致食物链断裂");
                v = v / r;
            }
        } else break;
    }
    return v;
}

double ExprEval::factor() {
    while (pos_ < s_.size() && isspace((unsigned char)s_[pos_])) pos_++;
    // 空因子 = 表达式意外结束（此前静默返回 0，现报错）
    if (pos_ >= s_.size())
        throw FclError(ErrCode::EXPR, "🌿 变异物种入侵，表达式意外结束");
    if (s_[pos_] == '(') {
        pos_++;
        double v = expr();
        while (pos_ < s_.size() && isspace((unsigned char)s_[pos_])) pos_++;
        if (pos_ >= s_.size() || s_[pos_] != ')')
            throw FclError(ErrCode::EXPR, "🌿 变异物种入侵，表达式括号未闭合");
        pos_++;
        return v;
    }
    if (s_.compare(pos_, 5, "MATCH") == 0) {
        pos_ += 5;
        while (pos_ < s_.size() && isspace((unsigned char)s_[pos_])) pos_++;
        if (pos_ >= s_.size() || s_[pos_] != '(')
            throw FclError(ErrCode::EXPR, "🌿 变异物种入侵，MATCH 后缺少左括号");
        pos_++;
        size_t st = pos_;
        while (pos_ < s_.size() && s_[pos_] != ')') pos_++;
        if (pos_ >= s_.size())
            throw FclError(ErrCode::EXPR, "🌿 变异物种入侵，MATCH 括号未闭合");
        std::string name = s_.substr(st, pos_ - st);
        pos_++;
        // 去掉首尾空白
        size_t a = name.find_first_not_of(" \t"), b = name.find_last_not_of(" \t");
        name = (a == std::string::npos) ? "" : name.substr(a, b - a + 1);
        // MATCH(name)：该物种是否发生过 MUTATION 变异（按物种根名判定，
        // 根名 / 成员名 / 变异后名称均可，如 Wolf / Wolf_M1 / Wolv_M1）
        std::string root = canonicalSpecies(speciesRoot(name));
        return (mut_ && !root.empty() && mut_->count(root)) ? 1.0 : 0.0;
    }
    // 数字解析：失败即抛错（P1-4 修复，不再静默返回 0）
    char* endp = nullptr;
    double v = strtod(s_.c_str() + pos_, &endp);
    if (endp == s_.c_str() + pos_)
        throw FclError(ErrCode::EXPR, "🌿 变异物种入侵，表达式含无法解析的内容");
    pos_ = endp - s_.c_str();
    return v;
}

} // namespace fcl
