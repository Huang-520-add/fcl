#pragma once
// ============================================================
//  FCL 表达式求值（递归下降）：
//  数字 / 分数 / + - * / ( ) / MATCH(name)
//  无法解析的 token 抛 Err::EXPR（不再静默返回 0）
// ============================================================
#include <string>
#include <map>
#include "fcl_error.h"

namespace fcl {

class ExprEval {
public:
    static double eval(const std::string& src, const std::map<std::string, bool>& mutated);
private:
    explicit ExprEval(const std::string& s) : s_(s) {}
    const std::string& s_;
    size_t pos_ = 0;
    const std::map<std::string, bool>* mut_ = nullptr;

    double expr();
    double term();
    double factor();
};

} // namespace fcl
