#pragma once
// ============================================================
//  FCL 解析器：三段式块内容 -> 语句列表
// ============================================================
#include <string>
#include <vector>
#include "fcl_error.h"

namespace fcl {

// 语句：简单指令或复合控制块
struct Stmt {
    bool hasBody = false;
    std::string kind;              // 简单语句: GMO/STORM/INTRODUCE/DEVOURS/CLONE/ASSESS/
                                   // SYMBIOSIS/COMPETITION/MIMICRY/ROT/SPROUT/EXTINCTION
    std::vector<std::string> args; // 原始 token（执行器按位置取参）
    std::string kw;                // 复合块: SEASON/MIGRATION/HIBERNATION/MUTATION/CASE
    std::string branch;            // SEASON 的 RAIN/DRY；CASE 的特征
    std::vector<Stmt> body;
    std::vector<Stmt> dryBody;     // SEASON 的 DRY 分支（与 RAIN 合并，二选一）
    bool hasDry = false;
    int line = 0;
    bool inDecay = false;          // 是否位于 DECAY 块（分解者豁免）
};

// 工具
std::string trim(const std::string& s);
std::vector<std::string> splitWS(const std::string& s);

// 块内容解析为语句列表（lineNo 为引用计数，跨块连续）
std::vector<Stmt> parseBlock(const std::string& src, int& lineNo);

} // namespace fcl
