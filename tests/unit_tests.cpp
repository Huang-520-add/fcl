// ============================================================
//  FCL 单元测试：直接测试 expr / ecology / parser 模块
//  （v3.0 新增：此前仅有端到端黑盒测试，模块级缺陷难以定位）
//  用法: make unit
// ============================================================
#include "expr.h"
#include "ecology.h"
#include "parser.h"
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

static int g_pass = 0, g_fail = 0;

#define CHECK(cond) do { \
    if (cond) { g_pass++; } \
    else { g_fail++; std::printf("FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); } \
} while (0)

// 带错误码校验的异常断言
#define CHECK_ERR(stmt, errcode) do { \
    bool caught = false; \
    try { stmt; } catch (const fcl::FclError& e) { \
        caught = true; \
        if (e.code == fcl::ErrCode::errcode) g_pass++; \
        else { g_fail++; std::printf("FAIL %s:%d  错误码 %s（期望 %s）\n", \
            __FILE__, __LINE__, e.codeStr().c_str(), #errcode); } \
    } \
    if (!caught) { g_fail++; std::printf("FAIL %s:%d  未抛出异常（期望 %s）\n", \
        __FILE__, __LINE__, #errcode); } \
} while (0)

using namespace fcl;

// ---------------- 表达式求值 ----------------
static void test_expr() {
    std::set<std::string> none;
    CHECK(std::fabs(ExprEval::eval("1+2*3", none) - 7.0) < 1e-9);
    CHECK(std::fabs(ExprEval::eval("(10+5)*2", none) - 30.0) < 1e-9);
    CHECK(std::fabs(ExprEval::eval("1/3", none) - 0.3333333333) < 1e-6);
    CHECK(std::fabs(ExprEval::eval("2*(3+(4-1))", none) - 12.0) < 1e-9);
    CHECK(std::fabs(ExprEval::eval("-3", none) + 3.0) < 1e-9);
    CHECK(std::fabs(ExprEval::eval("  60 + 5  ", none) - 65.0) < 1e-9);
    CHECK(std::fabs(ExprEval::eval("10/4", none) - 2.5) < 1e-9);

    // MATCH：变异集合含 Wolf → 1，否则 0
    std::set<std::string> mutated{ "Wolf" };
    CHECK(ExprEval::eval("MATCH(Wolf)", mutated) == 1.0);
    CHECK(ExprEval::eval("MATCH(Wolf_M1)", mutated) == 1.0);   // 成员名
    CHECK(ExprEval::eval("MATCH(Wolv_M1)", mutated) == 1.0);   // 变异后名称（canonical 还原）
    CHECK(ExprEval::eval("MATCH(Tiger)", mutated) == 0.0);
    CHECK(ExprEval::eval("4*(1-MATCH(Wolf))", mutated) == 0.0);

    // v3.0 修复：错误不再静默返回 0
    CHECK_ERR((ExprEval::eval("1/(3-3)", none)), DIVZERO);     // 除零
    CHECK_ERR((ExprEval::eval("(1+2", none)), EXPR);           // 括号未闭合
    CHECK_ERR((ExprEval::eval("1+", none)), EXPR);             // 表达式意外结束
    CHECK_ERR((ExprEval::eval("", none)), EXPR);               // 空表达式
    CHECK_ERR((ExprEval::eval("2 3", none)), EXPR);            // 残留 token
    CHECK_ERR((ExprEval::eval("MATCH Wolf", none)), EXPR);     // MATCH 缺 '('
    CHECK_ERR((ExprEval::eval("MATCH(Wolf", none)), EXPR);     // MATCH 括号未闭合
    CHECK_ERR((ExprEval::eval("abc", none)), EXPR);            // 非数字
}

// ---------------- 生态圈校验 ----------------
static void test_ecology() {
    // 在册物种表：10 物种，5 营养级
    CHECK(ECOLOGY.size() == 10);
    int perLevel[6] = { 0, 0, 0, 0, 0, 0 };
    for (const auto& s : ECOLOGY) perLevel[(int)s.type]++;
    CHECK(perLevel[1] == 2);  // PRODUCER
    CHECK(perLevel[2] == 2);  // HERBIVORE
    CHECK(perLevel[3] == 2);  // CARNIVORE
    CHECK(perLevel[4] == 2);  // APEX
    CHECK(perLevel[5] == 2);  // DECOMPOSER

    // 合法命名矩阵：独居物种 <名>_<编号>；群居物种 Alpha_ / _M / _F 三种形态
    for (const auto& s : ECOLOGY) {
        std::string solo = s.name + "_1";
        if (!s.social) CHECK(ecoNameError(solo, s.type).empty());
        if (s.social) {
            CHECK(ecoNameError("Alpha_" + s.name, s.type).empty());
            CHECK(ecoNameError(s.name + "_M1", s.type).empty());
            CHECK(ecoNameError(s.name + "_F7", s.type).empty());
            CHECK(!ecoNameError(s.name + "_1", s.type).empty());   // 群居缺 M/F 标签
        } else {
            CHECK(!ecoNameError("Alpha_" + s.name, s.type).empty()); // 独居伪造 Alpha
            CHECK(!ecoNameError(s.name + "_M1", s.type).empty());   // 独居伪造性别标签
        }
        // 营养级错配：任何物种用错误营养级登记都应报错
        Trophic wrong = (s.type == HERBIVORE) ? PRODUCER : HERBIVORE;
        CHECK(!ecoNameError(solo, wrong).empty());
    }

    // 外来物种
    CHECK(!ecoNameError("Zebra_1", HERBIVORE).empty());
    CHECK(!ecoNameError("Penguin_M1", CARNIVORE).empty());

    // 变异表：10 条完整映射，值互不冲突
    CHECK(mutationTable().size() == 10);
    for (const auto& [k, v] : mutationTable()) {
        CHECK(k != v);
        CHECK(!v.empty());
        CHECK(canonicalSpecies(k) == k);       // 原名 → 自身
        CHECK(canonicalSpecies(v) == k);       // 变异名 → 还原原名
    }
    CHECK(canonicalSpecies("Zebra").empty());  // 未知 → 空

    // 根名提取与 token 改写
    CHECK(speciesRoot("Wolf_M1") == "Wolf");
    CHECK(speciesRoot("Alpha_Wolf") == "Wolf");
    CHECK(speciesRoot("Wolf") == "Wolf");
    CHECK(renameSpeciesToken("Wolf_M1", "Wolf", "Wolv") == "Wolv_M1");
    CHECK(renameSpeciesToken("Alpha_Wolf", "Wolf", "Wolv") == "Alpha_Wolv");
    CHECK(renameSpeciesToken("Tiger_1", "Wolf", "Wolv") == "Tiger_1");  // 非目标物种不改
}

// ---------------- 解析器 ----------------
static void test_parser() {
    // 基础语句解析
    int ln = 1;
    auto stmts = parseBlock("INTRODUCE Grass_1 AS PRODUCER WITH 60 ;", ln);
    CHECK(stmts.size() == 1 && stmts[0].kind == "INTRODUCE" && stmts[0].args[1] == "Grass_1");

    // SEASON RAIN {...} DRY {...} 合并
    ln = 1;
    stmts = parseBlock("SEASON RAIN { INTRODUCE Grass_1 AS PRODUCER WITH 1 ; } DRY { INTRODUCE Grass_2 AS PRODUCER WITH 2 ; }", ln);
    CHECK(stmts.size() == 1 && stmts[0].kw == "SEASON" && stmts[0].hasDry);
    CHECK(stmts[0].body.size() == 1 && stmts[0].dryBody.size() == 1);

    // MUTATION + CASE：字符串内的 CASE 子串不切分（P2-5 修复）
    ln = 1;
    stmts = parseBlock(
        "MUTATION Wolf_M1 { "
        "CASE \"A CASE in string:\": INTRODUCE Grass_1 AS PRODUCER WITH 1 ; "
        "CASE \"normal\": INTRODUCE Grass_2 AS PRODUCER WITH 2 ; }", ln);
    CHECK(stmts.size() == 1 && stmts[0].kw == "MUTATION");
    CHECK(stmts[0].body.size() == 2);
    CHECK(stmts[0].body[0].branch == "A CASE in string:");
    CHECK(stmts[0].body[1].branch == "normal");

    // 字符串内的花括号不计入块深度（extractBlock 联动行为由 interpreter 测）
    ln = 1;
    stmts = parseBlock("MUTATION Wolf_M1 { CASE \"br{ace}\": INTRODUCE Grass_1 AS PRODUCER WITH 1 ; }", ln);
    CHECK(stmts.size() == 1 && stmts[0].body.size() == 1);
    CHECK(stmts[0].body[0].branch == "br{ace}");

    // v3.0 新原语解析
    ln = 1;
    stmts = parseBlock("SCENT Wolf_M1 TO Tiger_1 ; LURK Wolf_M1 FOR 10 ; POUNCE Wolf_M1 ;", ln);
    CHECK(stmts.size() == 3);
    CHECK(stmts[0].kind == "SCENT" && stmts[0].args[1] == "Wolf_M1" && stmts[0].args[3] == "Tiger_1");
    CHECK(stmts[1].kind == "LURK" && stmts[1].args[3] == "10");
    CHECK(stmts[2].kind == "POUNCE" && stmts[2].args[1] == "Wolf_M1");

    // v3.0 精确报错：缺 AGAINST / USING / TO
    CHECK_ERR((parseBlock("ASSESS Wolf_M1 Tiger_1 TO Lion_F1 ;", ln)), SYNTAX);
    CHECK_ERR((parseBlock("Wolf_M1 DEVOURS Sheep_M1 SUM ;", ln)), SYNTAX);
    CHECK_ERR((parseBlock("ROT Fungus_1 STDOUT ;", ln)), SYNTAX);
    CHECK_ERR((parseBlock("SCENT Wolf_M1 Tiger_1 ;", ln)), SYNTAX);
    CHECK_ERR((parseBlock("LURK Wolf_M1 10 ;", ln)), SYNTAX);
    CHECK_ERR((parseBlock("FOO BAR ;", ln)), SYNTAX);              // 未知关键字
    CHECK_ERR((parseBlock("INTRODUCE Grass_1 PRODUCER WITH 1 ;", ln)), SYNTAX);

    // 嵌套深度守卫（P2-7 修复）：65 层 MIGRATION 报错
    std::string deep;
    for (int i = 0; i < MAX_NEST_DEPTH + 1; i++) deep += "MIGRATION Grass_1 OVER 1 { ";
    deep += "INTRODUCE Grass_1 AS PRODUCER WITH 1 ;";
    for (int i = 0; i < MAX_NEST_DEPTH + 1; i++) deep += " }";
    ln = 1;
    CHECK_ERR((parseBlock(deep, ln)), SYNTAX);
}

int main() {
    std::printf("[expr]\n"); std::fflush(stdout);
    test_expr();
    std::printf("[ecology]\n"); std::fflush(stdout);
    test_ecology();
    std::printf("[parser]\n"); std::fflush(stdout);
    test_parser();
    std::printf("单元测试通过: %d / 失败: %d\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
