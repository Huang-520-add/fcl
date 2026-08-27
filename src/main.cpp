// ============================================================
//  FCL (FoodChain Language) v2.2 —— 入口
//  食物链语言：代码在吞噬中传递，真理在分解中显现。
//  用法: fcl [--slow] <程序.fc>
//  编译: g++ -std=c++17 -O2 -o fcl src/*.cpp
// ============================================================
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "interpreter.h"

int main(int argc, char** argv) {
    bool slow = false;
    std::string path;

    // 参数解析：--slow 生态慢放（GC 模拟微生物分解速度）
    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        if (a == "--slow") slow = true;
        else path = a;
    }

    if (path.empty()) {
        std::cerr << "用法: fcl [--slow] <程序文件.fc>" << std::endl;
        std::cerr << "FCL 食物链语言 v2.2 —— 代码在吞噬中传递，真理在分解中显现。" << std::endl;
        return 1;
    }

    std::ifstream f(path);
    if (!f) {
        std::cerr << "无法打开文件: " << path << std::endl;
        return 1;
    }
    std::stringstream buf;
    buf << f.rdbuf();

    try {
        fcl::Interp ip;
        ip.setSlow(slow);
        ip.run(buf.str());
    } catch (fcl::Error& e) {
        // 结构化错误输出：错误码 + 行号 + 生态报错
        std::cout << "[" << e.codeStr();
        if (e.line > 0) std::cout << " @第" << e.line << "行";
        std::cout << "] " << e.msg << std::endl;
        return 1;
    }
    return 0;
}
