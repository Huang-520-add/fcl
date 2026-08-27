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
        if (tag.size() >= 2 && (tag[0] == 'M' || tag[0] == 'F') && isdigit((unsigned char)tag[1])) return "";
        return "⚠️ 族谱登记混乱！群居物种须为 Alpha_<物种> 或 <物种>_<M/F><编号>";
    }
    bool ok = !tag.empty();
    for (char c : tag) if (!isdigit((unsigned char)c)) { ok = false; break; }
    if (ok) return "";
    return "⚠️ 族谱登记混乱！该物种须为 <物种>_<编号>";
}

} // namespace fcl
