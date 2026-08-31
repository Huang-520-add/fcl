// ============================================================
//  FCL (FoodChain Language) v3.0.2 —— 入口
// ============================================================
#include "interpreter.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
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
            std::cout << "FCL 食物链语言 v3.0.2 —— 代码在吞噬中传递，真理在分解中显现。" << std::endl;
            return 0;
        } else if (a == "--seed" && i + 1 < argc) {
            // A5 修复：--seed 非数字时不再抛出未捕获异常导致进程 abort
            try {
                seed = (unsigned)std::stoul(argv[++i]);
                hasSeed = true;
            } catch (...) {
                std::cerr << "❌ --seed 需要非负整数" << std::endl;
                return 1;
            }
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
    // C6 修复：剥离 UTF-8 BOM（EF BB BF），否则首行解析会带乱码字节
    if (src.size() >= 3 &&
        (unsigned char)src[0] == 0xEF && (unsigned char)src[1] == 0xBB && (unsigned char)src[2] == 0xBF)
        src.erase(0, 3);

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
