#pragma once
// ============================================================
//  FCL 解析器：三段式块内容 -> 语句列表
//  v3.0：精确 token 校验（报错指出期望 token 与实际 token）、
//        嵌套深度守卫、字符串感知的 CASE 切分、新原语 SCENT/LURK/POUNCE
// ============================================================
#include <string>
#include <vector>
#include "fcl_error.h"
#include "lexer.h"

namespace fcl {

// 语句：简单指令或复合控制块
struct Stmt {
    bool hasBody = false;
    std::string kind;              // 简单语句: GMO/STORM/INTRODUCE/DEVOURS/CLONE/ASSESS/
                                   // SYMBIOSIS/COMPETITION/MIMICRY/ROT/SCENT/LURK/POUNCE/EXTINCTION
                                   // + v3.1: FORWARD/BACKWARD/BUMP/LOAD/STORE
    std::vector<std::string> args; // 原始 token（执行器按位置取参；与词法器输出一致）
    std::vector<Token> toks;       // v3.1：类型化 token 流（AST 节点携带的词法信息）
    std::string kw;                // 复合块: SEASON/MIGRATION/HIBERNATION/MUTATION/CASE
                                   // + v3.1: WHILE
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
// depth 为控制块嵌套深度（内部使用，超过 MAX_NEST_DEPTH 抛 FCL-0001）
const int MAX_NEST_DEPTH = 64;
std::vector<Stmt> parseBlock(const std::string& src, int& lineNo, int depth = 0);

} // namespace fcl
