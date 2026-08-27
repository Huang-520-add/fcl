#pragma once
// ============================================================
//  FCL 错误系统：错误码 + 中文生态报错 + 行号
// ============================================================
#include <string>

namespace fcl {

enum class ErrCode {
    SYNTAX,      // 🌿 变异物种入侵，语法免疫系统失效
    TROPHIC,     // 🦴 食性冲突，捕食者拒绝进食
    INVASIVE,    // 🌿 外来物种入侵，生态圈不予接纳！
    TAXONOMY,    // ⚠️ 分类学混乱！
    GENEALOGY,   // ⚠️ 族谱登记混乱！
    STRUCTURE,   // 🌍 生态崩溃，食物链断裂！
    DIVZERO,     // 🔥 干旱导致食物链断裂
    STARVE,      // 🥀 捕食者饿死，能量为负
    HERB_OVERFLOW,    // 🤢 胃溃疡溢出，能量归零
    TIMEOUT,     // ⏰ 冬眠过久，强制唤醒
    EXPR,        // 表达式解析失败
    UNKNOWN,
};

struct FclError {
    ErrCode code = ErrCode::UNKNOWN;
    std::string msg;
    int line = -1;   // 出错语句行号（-1 = 未知）

    FclError() = default;
    FclError(ErrCode c, std::string m) : code(c), msg(std::move(m)) {}
    FclError(ErrCode c, std::string m, int l) : code(c), msg(std::move(m)), line(l) {}

    // 错误码文本（FCL-0001 格式）
    std::string codeStr() const {
        switch (code) {
            case ErrCode::SYNTAX:    return "FCL-0001";
            case ErrCode::TROPHIC:   return "FCL-0002";
            case ErrCode::INVASIVE:  return "FCL-0003";
            case ErrCode::TAXONOMY:  return "FCL-0004";
            case ErrCode::GENEALOGY: return "FCL-0005";
            case ErrCode::STRUCTURE: return "FCL-0006";
            case ErrCode::DIVZERO:   return "FCL-0007";
            case ErrCode::STARVE:    return "FCL-0008";
            case ErrCode::HERB_OVERFLOW:  return "FCL-0009";
            case ErrCode::TIMEOUT:   return "FCL-0010";
            case ErrCode::EXPR:      return "FCL-0011";
            default:             return "FCL-0000";
        }
    }
};

} // namespace fcl
