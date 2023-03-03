#include "src/base/log/logging.h"
#include "src/base/env.h"
using namespace zws;

Logger::ptr g_logger = LOG_ROOT();

Env *g_env = EnvMgr::GetInstance();

int main(int argc, char *argv[]) {
    g_env->addHelp("h", "print this help message");

    bool is_print_help = false;
    if(!g_env->init(argc, argv)) {
        is_print_help = true;
    }
    if(g_env->has("h")) {
        is_print_help = true;
    }

    if(is_print_help) {
        g_env->printHelp();
        return false;
    }

    // LOG_INFO(g_logger)<< "exe: " << g_env->getExe();
    // LOG_INFO(g_logger) <<"cwd: " << g_env->getCwd();
    // LOG_INFO(g_logger) << "absoluth path of test: " << g_env->getAbsolutePath("test");
    // LOG_INFO(g_logger) << "conf path:" << g_env->getConfigPath();

    // g_env->add("key1", "value1");
    // LOG_INFO(g_logger) << "key1: " << g_env->get("key1");

    // g_env->setEnv("key2", "value2");
    // LOG_INFO(g_logger) << "key2: " << g_env->getEnv("key2");

    // LOG_INFO(g_logger) << g_env->getEnv("PATH");

    return 0;
}