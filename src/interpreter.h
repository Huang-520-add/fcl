#pragma once
// ============================================================
//  FCL 解释器：生态执行引擎
//  修复：STORM 显式开关 / MUTATION 全物种 / Windows 超时 /
//        REAL/CODE 双运行模式 / 错误行号
// ============================================================
#include <string>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <random>

#include "fcl_error.h"
#include "ecology.h"
#include "expr.h"
#include "parser.h"

namespace fcl {

struct Variable {
    std::string name;
    Trophic type;
    double value;
    int age = 0;          // 未被引用轮数（GC 用）
    int born = 0;         // 创建时的指令序号（GC 适应期）
    bool alive = true;
};

class Interp {
public:
    void run(const std::string& raw);
    void setRealMode(bool v) { realMode_ = v; }  // 真实模式：生态等待生效（进食/分解/溢出惩罚）

private:
    std::map<std::string, Variable> vars_;
    std::vector<std::string> varOrder_;
    std::map<std::string, bool> mutated_;
    std::map<std::string, bool> rotFired_;
    std::deque<bool> humidity_;   // 最近 3 条指令湿度记录
    std::set<std::string> touchedThis_;
    int blockDepth_ = 0;          // 复合控制块嵌套深度（块内暂停 GC）
    bool biomePhase_ = false;     // BIOME 引种段（创造阶段，不触发分解）
    int stmtCount_ = 0;
    bool gmo_ = false;
    bool storm_ = false;          // STORM ENABLED：RAIN>3 条语句随机乱序
    bool numericOut_ = false;     // NUMERIC OUTPUT：ROT 直接输出数值（默认编码模式）
    bool realMode_ = false;       // REAL MODE：生态等待生效（默认 CODE 快速模式）
    std::mt19937 rng_{ std::random_device{}() };

    // 预处理与结构
    std::string stripComments(const std::string& raw);
    std::string extractBlock(const std::string& src, const std::string& name, size_t from);

    // 变量
    Variable& getVar(const std::string& name);
    void touch(const std::string& name);
    size_t addrOf(const std::string& name);
    Trophic parseTrophic(const std::string& s);

    // 执行
    void execStmts(std::vector<Stmt>& stmts);
    void execOne(Stmt& s);
    void humidityPush(bool isDevour);
    int humidityTrue();

    // 指令实现
    void execIntroduce(Stmt& s);
    void execDevour(Stmt& s);
    void execClone(Stmt& s);
    void requireAPEX(const std::string& name);
    void execAssess(Stmt& s);
    void execSymbiosis(Stmt& s);
    void execCompetition(Stmt& s);
    void execMimicry(Stmt& s);
    void execRot(Stmt& s);
    void execSprout(Stmt& s);
    double readNumWithTimeout(int ms);
    void execExtinction(Stmt& s);

    // 控制流
    static int countStmts(const std::vector<Stmt>& v);
    void execSeason(Stmt& s);
    void execMigration(Stmt& s);
    void execHibernation(Stmt& s);
    void execMutation(Stmt& s);
    static void renameVar(std::vector<Stmt>& v, const std::string& from, const std::string& to);

    // 垃圾回收
    void gcTick();
};

} // namespace fcl
