#pragma once
// ============================================================
//  FCL 表达式求值（递归下降）：
//  数字 / 分数 / + - * / ( ) / MATCH(name)
//  无法解析的 token 抛 ErrCode::EXPR（不再静默返回 0）
// ============================================================
#include <string>
#include <set>
#include "fcl_error.h"

namespace fcl {

class ExprEval {
public:
    // mutatedRoots：发生过 MUTATION 变异的物种根名集合（Wolf / Wolv 均指 Wolf）
    static double eval(const std::string& src, const std::set<std::string>& mutatedRoots);
private:
    explicit ExprEval(const std::string& s) : s_(s) {}
    const std::string& s_;
    size_t pos_ = 0;
    const std::set<std::string>* mut_ = nullptr;

    double expr();
    double term();
    double factor();
};

} // namespace fcl
