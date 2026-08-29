// ============================================================
//  FCL (FoodChain Language) v3.0.0 —— 入口
// ============================================================
#include "interpreter.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "用法: fcl [选项] <程序.fc>\n"
                  << "选项:\n"
                  << "  --real       真实模式（生态等待生效：进食/分解/潜伏按真实节奏）\n"
                  << "  --seed <n>   固定随机种子（确定性复现 MUTATION/STORM，测试用）\n"
                  << "  --version    显示版本\n";
        return 1;
    }
    bool realMode = false;
    unsigned seed = 0;
    bool hasSeed = false;
    std::string path;
    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        if (a == "--real") realMode = true;
        else if (a == "--version") {
            std::cout << "FCL 食物链语言 v3.0.0 —— 代码在吞噬中传递，真理在分解中显现。" << std::endl;
            return 0;
        } else if (a == "--seed" && i + 1 < argc) {
            seed = (unsigned)std::stoul(argv[++i]);
            hasSeed = true;
        } else path = a;
    }
    if (path.empty()) {
        std::cerr << "用法: fcl [选项] <程序.fc>（--real / --seed <n> / --version）" << std::endl;
        return 1;
    }
    std::ifstream in(path);
    if (!in) {
        std::cerr << "❌ 无法打开文件: " << path << std::endl;
        return 1;
    }
    std::string src((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    fcl::Interp interp;
    if (realMode) interp.setRealMode(true);
    if (hasSeed) interp.setSeed(seed);
    try {
        interp.run(src);
    } catch (const fcl::FclError& e) {
        std::cout << "\n[" << e.codeStr();
        if (e.line > 0) std::cout << " @第" << e.line << "行";
        std::cout << "] " << e.msg << std::endl;
        return 1;
    }
    return 0;
}
