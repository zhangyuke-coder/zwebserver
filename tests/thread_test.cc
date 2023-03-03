/**
 * @file test_thread.cc
 * @brief 线程模块测试
 * @version 0.1
 * @date 2021-06-15
 */
#include "src/zws.h"

Logger::ptr g_logger = LOG_ROOT();

int count = 0;
Mutex s_mutex;

void func1(void *arg) {
    LOG_INFO(g_logger) << "name:" << Thread::GetName()
        << " this.name:" << Thread::GetThis()->getName()
        << " thread name:" << GetThreadName()
        << " id:" << GetThreadId()
        << " this.id:" << Thread::GetThis()->getId();
    LOG_INFO(g_logger) << "arg: " << *(int*)arg;
    for(int i = 0; i < 10000; i++) {
        Mutex::Lock lock(s_mutex);
        ++count;
    }
}

int main(int argc, char *argv[]) {
    EnvMgr::GetInstance()->init(argc, argv);
    Config::LoadFromConfDir(EnvMgr::GetInstance()->getConfigPath());

    std::vector<Thread::ptr> thrs;
    int arg = 123456;
    for(int i = 0; i < 3; i++) {
        // 带参数的线程用std::bind进行参数绑定
        Thread::ptr thr(new Thread(std::bind(func1, &arg), "thread_" + std::to_string(i)));
        thrs.push_back(thr);
    }

    for(int i = 0; i < 3; i++) {
        thrs[i]->join();
    }
    
    LOG_INFO(g_logger) << "count = " << count;
    return 0;
}

