// ============================================================
//  FCL WebAssembly 适配层 — v2.4.0（重建时间戳触发 CI）
// ============================================================
#include <emscripten.h>
#include <iostream>
#include <sstream>
#include <string>
#include "interpreter.h"
#include "fcl_error.h"

extern "C" {

EMSCRIPTEN_KEEPALIVE
const char* fcl_run(const char* src) {
    static std::string result;
    std::stringstream buf;
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

EMSCRIPTEN_KEEPALIVE
const char* fcl_version() {
    return "v2.4.0";
}

} // extern "C"
