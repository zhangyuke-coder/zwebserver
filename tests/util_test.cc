/**
 * @file test_util.cpp
 * @brief util与macro测试
 * @version 0.1
 * @date 2021-06-12
 */
#include "src/base/log/logging.h"
#include "src/base/env.h"
#include "src/base/macro.h"
using namespace zws;
Logger::ptr g_logger = LOG_ROOT();

void test2() {
    std::cout << BacktraceToString() << std::endl;
}
void test1() {
    test2();
}

void test_backtrace() {
    test1();
}

int main() {
    LOG_INFO(g_logger) << GetCurrentMS();
    LOG_INFO(g_logger) << GetCurrentUS();
    LOG_INFO(g_logger) << ToUpper("hello");
    LOG_INFO(g_logger) << ToLower("HELLO");
    LOG_INFO(g_logger) << Time2Str();
    LOG_INFO(g_logger) << Str2Time("1970-01-01 00:00:00"); // -28800

    std::vector<std::string> files;
    FSUtil::ListAllFile(files, "./sylar", ".cpp");
    for (auto &i : files) {
        LOG_INFO(g_logger) << i;
    }

    // todo, more...

    test_backtrace();

    ASSERT2(false, "assert");
    return 0;
}