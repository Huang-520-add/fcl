// ============================================================
//  FCL (FoodChain Language) v2.4.0 —— 入口
//  食物链语言：代码在吞噬中传递，真理在分解中显现。
//  用法: fcl [--real] <程序.fc>
//  编译: g++ -std=c++17 -O2 -o fcl src/*.cpp
// ============================================================
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "interpreter.h"

int main(int argc, char** argv) {
    bool realMode = false;
    std::string path;

    // 参数解析：--slow / --real 真实模式（生态等待生效）
    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        if (a == "--slow" || a == "--real") realMode = true;
        else path = a;
    }

    if (path.empty()) {
        std::cerr << "用法: fcl [--slow|--real] <程序文件.fc>" << std::endl;
        std::cerr << "FCL 食物链语言 v2.4.0 —— 代码在吞噬中传递，真理在分解中显现。" << std::endl;
        std::cerr << "运行模式: 默认 CODE（快速）| --real 真实模式（生态等待生效）" << std::endl;
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
        ip.setRealMode(realMode);
        ip.run(buf.str());
    } catch (fcl::FclError& e) {
        // 结构化错误输出：错误码 + 行号 + 生态报错
        std::cout << "[" << e.codeStr();
        if (e.line > 0) std::cout << " @第" << e.line << "行";
        std::cout << "] " << e.msg << std::endl;
        return 1;
    }
    return 0;
}
