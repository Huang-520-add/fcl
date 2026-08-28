// ============================================================
//  FCL WebAssembly 适配层
//  将 C++ 解释器编译为 WASM，供浏览器 Playground 调用
//  编译: emcc -O2 -sMODULARIZE=1 -sEXPORT_NAME=createFcl -sWASM=1 \
//        -sALLOW_MEMORY_GROWTH -sEXPORTED_FUNCTIONS=_fcl_run, \
//        web_main.cpp src/*.cpp -o web/fcl.js
// ============================================================
//  版本: v2.4.0 — 2026-08-28
// ============================================================
#include <emscripten.h>
#include <iostream>
#include <sstream>
#include <string>

#include "interpreter.h"
#include "fcl_error.h"

extern "C" {

// 运行一段 FCL 源码，返回捕获的完整输出（stdout + 错误信息）
EMSCRIPTEN_KEEPALIVE
const char* fcl_run(const char* src) {
    static std::string result;
    std::stringstream buf;
    // 重定向 cout 到 stringstream，捕获解释器输出
    std::streambuf* old = std::cout.rdbuf(buf.rdbuf());
    try {
        fcl::Interp ip;
        ip.run(src);
    } catch (fcl::FclError& e) {
        buf << "\n[" << e.codeStr();
        if (e.line > 0) buf << " @第" << e.line << "行";
        buf << "] " << e.msg;
    } catch (...) {
        buf << "\n[FCL-0000] 未知错误";
    }
    std::cout.rdbuf(old);
    result = buf.str();
    return result.c_str();
}

// 版本号
EMSCRIPTEN_KEEPALIVE
const char* fcl_version() {
    return "v2.4.0";
}

} // extern "C"
