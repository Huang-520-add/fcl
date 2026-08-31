#pragma once
// ============================================================
//  FCL 词法分析器（分词器）
//  v3.1：独立分词模块 —— 此前解析器内联 splitWS 做空白切分，
//        现抽出为类型化 Token 流，供解析器（AST 构建）与未来工具复用。
//  tokenize() 与历史 splitWS 输出逐字一致，保证解析 / 单测兼容。
// ============================================================
#include <string>
#include <vector>

namespace fcl {

enum class TokType {
    KW,        // 关键字（INTRODUCE / DEVOURS / WHILE ...）
    IDENT,     // 标识符（物种名、变量名）
    NUM,       // 数字字面量
    STR,       // 字符串字面量（含引号）
    LBRACE,    // {
    RBRACE,    // }
    SEMI,      // ;
    OP,        // 运算符 ( ) + - * / =
    COLON,     // :
    UNKNOWN,
};

struct Token {
    TokType type = TokType::UNKNOWN;
    std::string text;
    int line = 0;
    bool isKw() const { return type == TokType::KW; }
};

// 语句级分词（替代 splitWS）：按空白切分，引号内空白保留（引号字符也留在
// token 中），与 splitWS 逐字一致。每个 token 标注类型，供 AST / 工具使用。
std::vector<Token> tokenize(const std::string& s);

// 全量结构化分词：额外识别 { } ; : 与运算符，用于未来解析 / 工具。
std::vector<Token> tokenizeFull(const std::string& src);

} // namespace fcl
