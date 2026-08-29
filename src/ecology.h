#pragma once
// ============================================================
//  FCL 生态圈：营养级、在册物种、命名校验、物种变异表
// ============================================================
#include <string>
#include <vector>
#include <map>

namespace fcl {

enum Trophic { PRODUCER = 1, HERBIVORE = 2, CARNIVORE = 3, APEX = 4, DECOMPOSER = 5 };

struct EcoSpecies {
    std::string name;
    Trophic type;
    bool social;  // 群居（有 Alpha 首领 + 性别成员）
};

// 在册物种表（10 物种，5 营养级）
extern const std::vector<EcoSpecies> ECOLOGY;

// 营养级中文名
std::string trophicName(Trophic t);

// 生态圈命名校验：返回空串 = 合法；否则为错误信息
// 格式：
//   Alpha_<物种>        群居首领（狼王 Alpha_Wolf）
//   <物种>_<M|F><编号>  群居成员（Wolf_M1 雄性1号）
//   <物种>_<编号>       独居/种群/菌落（Tiger_1、Grass_1、Fungus_1）
std::string ecoNameError(const std::string& name, Trophic type);

// 物种变异表（根名 → 变异根名，全部 10 个在册物种覆盖）
const std::map<std::string, std::string>& mutationTable();

// 提取物种根名：Wolf_M1 / Alpha_Wolf / Wolf → Wolf
std::string speciesRoot(const std::string& name);

// 根名规范化：在册物种返回自身；变异名还原为原名（Wolv → Wolf）；未知返回空串
std::string canonicalSpecies(const std::string& root);

// 物种级 token 改写（Wolf→Wolv 时同步改名块内引用）：
//   Wolf_M1 → Wolv_M1   Alpha_Wolf → Alpha_Wolv   Wolf → Wolv
std::string renameSpeciesToken(const std::string& tok, const std::string& from, const std::string& to);

} // namespace fcl
