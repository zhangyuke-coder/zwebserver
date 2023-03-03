#pragma once

#include <memory>
#include <functional>
#include "coctx.h"
#include "runtime.h"
namespace zws {

int getCoroutineIndex();

RunTime* getCurrentRunTime();

void setCurrentRunTime(RunTime* v);

class Coroutine : public std::enable_shared_from_this<Coroutine>{


/**
 * @brief 协程状态
 * @details 在sylar基础上进行了状态简化，只定义三态转换关系，也就是协程要么正在运行(RUNNING)，
 * 要么准备运行(READY)，要么运行结束(TERM)。不区分协程的初始状态，初始即READY。不区分协程是异常结束还是正常结束，
 * 只要结束就是TERM状态。也不区别HOLD状态，协程只要未结束也非运行态，那就是READY状态。
 */
public:
    enum State {
        /// 就绪态，刚创建或者yield之后的状态
        READY,
        /// 运行态，resume之后的状态
        RUNNING,
        /// 结束态，协程的回调函数执行完之后为TERM状态
        TERM
    };
private:

public:
    typedef std::shared_ptr<Coroutine> ptr;
        Coroutine();
    Coroutine(int size, char* stack_ptr);

    Coroutine(int size, char* stack_ptr, std::function<void()> cb);

    ~Coroutine();

    void init();
    int getCorId() const {
        return m_cor_id;
    }

    void setIsInCoFunc(const bool v) {
        m_is_in_cofunc = v;
    }

    bool getIsInCoFunc() const {
        return m_is_in_cofunc;
    }

    std::string getMsgNo() {
        return m_msg_no;
    }

    RunTime* getRunTime() {
        return &m_run_time; 
    }

    void setMsgNo(const std::string& msg_no) {
        m_msg_no = msg_no;
    }

    void setIndex(int index) {
        m_index = index;
    }

    int getIndex() {
        return m_index;
    }

    char* getStackPtr() {
        return m_stack_sp;
    }

    int getStackSize() {
        return m_stack_size;
    }

    void setCanResume(bool v) {
        m_can_resume = v;
    }
    bool setCallBack(std::function<void()> cb); 
public:
    static void Yield();

    static void Resume(Coroutine::ptr cor);

    // static Coroutine* GetCurrentCoroutine();
    static Coroutine::ptr GetCurrentCoroutine();
    static Coroutine::ptr GetMainCoroutine();

    static bool IsMainCoroutine();
    void reset(std::function<void()> cb);
    State getState() const { return m_state; }
public:
    std::function<void()> m_call_back;   

private:
    int m_cor_id {0};        // coroutine' id
    coctx m_coctx;           // coroutine regs
    int m_stack_size {0};         // size of stack memory space
    char* m_stack_sp {NULL};      // coroutine's stack memory space, you can malloc or mmap get some mermory to init this value
    bool m_is_in_cofunc {false};  // true when call CoFunction, false when CoFunction finished
    std::string m_msg_no;
    RunTime m_run_time;
    /// 协程状态
    State m_state        = READY;
    bool m_can_resume {true};

    int m_index {-1};             // index in coroutine pool
};
}