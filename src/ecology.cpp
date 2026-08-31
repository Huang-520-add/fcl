#include "ecology.h"
#include <cctype>

namespace fcl {

const std::vector<EcoSpecies> ECOLOGY = {
    {"Grass",    PRODUCER,   false},  // 草（种群）
    {"Algae",    PRODUCER,   false},  // 藻（种群）
    {"Sheep",    HERBIVORE,  true},   // 羊（群居，头羊 Alpha_Sheep）
    {"Rabbit",   HERBIVORE,  false},  // 兔（独居）
    {"Wolf",     CARNIVORE,  true},   // 狼（群居，狼王 Alpha_Wolf）
    {"Fox",      CARNIVORE,  false},  // 狐（独居）
    {"Tiger",    APEX,       false},  // 虎（独居）
    {"Lion",     APEX,       true},   // 狮（群居，狮王 Alpha_Lion）
    {"Fungus",   DECOMPOSER, false},  // 真菌（菌落）
    {"Bacillus", DECOMPOSER, false},  // 杆菌（菌落）
};

std::string trophicName(Trophic t) {
    switch (t) {
        case PRODUCER:   return "生产者";
        case HERBIVORE:  return "食草动物";
        case CARNIVORE:  return "食肉动物";
        case APEX:       return "顶级掠食者";
        case DECOMPOSER: return "分解者";
    }
    return "未知";
}

std::string ecoNameError(const std::string& name, Trophic type) {
    // Alpha_<物种>：群居首领
    if (name.rfind("Alpha_", 0) == 0) {
        std::string sp = name.substr(6);
        for (const auto& e : ECOLOGY) {
            if (e.name == sp) {
                if (!e.social) return "⚠️ 族谱登记混乱！" + sp + " 是独居物种，没有首领";
                if (e.type != type)
                    return "⚠️ 分类学混乱！" + sp + " 是" + trophicName(e.type) + "，不是" + trophicName(type);
                return "";
            }
        }
        return "🌿 外来物种入侵，生态圈不予接纳！";
    }
    // <物种>_<标识>
    size_t us = name.find('_');
    if (us == std::string::npos) return "⚠️ 族谱登记混乱！生态圈物种必须带族群标识";
    std::string sp = name.substr(0, us);
    std::string tag = name.substr(us + 1);
    const EcoSpecies* spc = nullptr;
    for (const auto& e : ECOLOGY) if (e.name == sp) { spc = &e; break; }
    if (!spc) return "🌿 外来物种入侵，生态圈不予接纳！";
    if (spc->type != type)
        return "⚠️ 分类学混乱！" + sp + " 是" + trophicName(spc->type) + "，不是" + trophicName(type);
    if (spc->social) {
        if (tag == "Alpha") return "";
        // A3 修复：群居成员必须严格为 <M|F><纯数字>，禁止尾随垃圾（如 Wolf_M1X 此前被放行）
        if (tag.size() >= 2 && (tag[0] == 'M' || tag[0] == 'F') && std::isdigit((unsigned char)tag[1])) {
            bool allDigits = true;
            for (size_t i = 1; i < tag.size(); i++)
                if (!std::isdigit((unsigned char)tag[i])) { allDigits = false; break; }
            if (allDigits) return "";
        }
        return "⚠️ 族谱登记混乱！群居物种须为 Alpha_<物种> 或 <物种>_<M/F><编号>";
    }
    bool ok = !tag.empty();
    for (char c : tag) if (!isdigit((unsigned char)c)) { ok = false; break; }
    if (ok) return "";
    return "⚠️ 族谱登记混乱！该物种须为 <物种>_<编号>";
}

// ============================================================
//  物种变异（MUTATION 共享设施）
// ============================================================
const std::map<std::string, std::string>& mutationTable() {
    // P0-2 修复：变异表覆盖全部 10 个在册物种（单一事实来源，文档/解释器共用）
    static const std::map<std::string, std::string> mut = {
        {"Grass", "Grasse"},   {"Algae", "Algee"},
        {"Sheep", "Sheepe"},   {"Rabbit", "Rabbite"},
        {"Wolf", "Wolv"},      {"Fox", "Foxy"},
        {"Tiger", "Tygre"},    {"Lion", "Lyone"},
        {"Fungus", "Funge"},   {"Bacillus", "Bacilluz"}
    };
    return mut;
}

std::string speciesRoot(const std::string& name) {
    if (name.rfind("Alpha_", 0) == 0) return name.substr(6);
    size_t us = name.find('_');
    return (us == std::string::npos) ? name : name.substr(0, us);
}

std::string canonicalSpecies(const std::string& root) {
    for (const auto& e : ECOLOGY) if (e.name == root) return root;
    // 变异名还原为原名（Wolv → Wolf）
    for (const auto& [from, to] : mutationTable()) if (to == root) return from;
    return "";
}

std::string renameSpeciesToken(const std::string& tok, const std::string& from, const std::string& to) {
    if (tok == from) return to;
    if (tok.rfind("Alpha_" + from, 0) == 0) return "Alpha_" + to + tok.substr(6 + from.size());
    if (tok.rfind(from + "_", 0) == 0) return to + tok.substr(from.size());
    return tok;
}

} // namespace fcl
