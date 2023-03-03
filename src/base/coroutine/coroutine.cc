#include <atomic>
#include <string.h>
#include <assert.h>
#include "coroutine.h"
// #include "zrpc/base/logging.h"
namespace zws {

// main coroutine, every io thread have a main_coroutine
static thread_local Coroutine::ptr t_main_coroutine = NULL;

// current thread is runing which coroutine
static thread_local Coroutine::ptr t_cur_coroutine = NULL;

static thread_local RunTime* t_cur_run_time = NULL;


// static thread_local bool t_enable_coroutine_swap = true;

static std::atomic_int t_coroutine_count {0};

static std::atomic_int t_cur_coroutine_id {1};


int getCoroutineIndex() {
  return t_cur_coroutine_id;
}

RunTime* getCurrentRunTime() {
  return t_cur_run_time;
}

void setCurrentRunTime(RunTime* v) {
  t_cur_run_time = v;
}


void CoFunction(Coroutine* co) {

    if (co!= nullptr) {
        co->setIsInCoFunc(true);

        // 去执行协程回调函数
        co->m_call_back();

        co->setIsInCoFunc(false);
    }

    // here coroutine's callback function finished, that means coroutine's life is over. we should yiled main couroutine
    Coroutine::Yield();
}




Coroutine::Coroutine() {
    // main coroutine'id is 0
    m_cor_id = 0;
    t_coroutine_count++;
    memset(&m_coctx, 0, sizeof(m_coctx));
    // t_cur_coroutine = enable_shared_from_this();
    // t_cur_coroutine = std::make_shared<Coroutine>(this);
    // DebugLog << "coroutine[" << m_cor_id << "] create";
}

Coroutine::Coroutine(int size, char* stack_ptr) : m_stack_size(size), m_stack_sp(stack_ptr) {
    assert(stack_ptr);

    if (!t_main_coroutine) {
        t_main_coroutine = std::make_shared<Coroutine>();
    }

    m_cor_id = t_cur_coroutine_id++;
    t_coroutine_count++;
    // DebugLog << "coroutine[" << m_cor_id << "] create";
}


Coroutine::Coroutine(int size, char* stack_ptr, std::function<void()> cb)
    : m_stack_size(size), m_stack_sp(stack_ptr) {

    assert(m_stack_sp);

    if (!t_main_coroutine) {
        t_main_coroutine = std::make_shared<Coroutine>();
    }

    setCallBack(cb);
    m_cor_id = t_cur_coroutine_id++;
    t_coroutine_count++;
    // DebugLog << "coroutine[" << m_cor_id << "] create";
}


bool Coroutine::setCallBack(std::function<void()> cb) {

    if (this == t_main_coroutine.get()) {
        // ErrorLog << "main coroutine can't set callback";
        return false;
    }
    if (m_is_in_cofunc) {
        // ErrorLog << "this coroutine is in CoFunction";
        return false;
    }

    m_call_back = cb;

    // assert(m_stack_sp != nullptr);

    char* top = m_stack_sp + m_stack_size;
    // first set 0 to stack
    // memset(&top, 0, m_stack_size);

    top = reinterpret_cast<char*>((reinterpret_cast<unsigned long>(top)) & -16LL);

    memset(&m_coctx, 0, sizeof(m_coctx));

    m_coctx.regs[kRSP] = top;
    m_coctx.regs[kRBP] = top;
    m_coctx.regs[kRETAddr] = reinterpret_cast<char*>(CoFunction); 
    m_coctx.regs[kRDI] = reinterpret_cast<char*>(this);

    m_can_resume = true;

    return true;

}

void Coroutine::reset(std::function<void()> cb) {

    m_call_back = cb;
    m_state = READY;
}
Coroutine::~Coroutine() {
    t_coroutine_count--;
    // DebugLog << "coroutine[" << m_cor_id << "] die";
}


Coroutine::ptr Coroutine::GetCurrentCoroutine() {
    if (t_cur_coroutine == nullptr) {
        t_main_coroutine = std::make_shared<Coroutine>();
        t_cur_coroutine = t_main_coroutine;
        
    }
    return t_cur_coroutine;
}

Coroutine::ptr Coroutine::GetMainCoroutine() {
    if (t_main_coroutine) {
        return t_main_coroutine;
    }
    t_main_coroutine = std::make_shared<Coroutine>();
    return t_main_coroutine;
}

bool Coroutine::IsMainCoroutine() {
    if (t_main_coroutine == nullptr || t_cur_coroutine == t_main_coroutine) {
        return true;
    }
    return false;
}







/********
form target coroutine back to main coroutine
********/
void Coroutine::Yield() {
    // if (!t_enable_coroutine_swap) {
    //   ErrorLog << "can't yield, because disable coroutine swap";
    //   return;
    // }
    if (t_main_coroutine == nullptr) {
        // ErrorLog << "main coroutine is nullptr";
        return;
    }

    if (t_cur_coroutine == t_main_coroutine) {
        // ErrorLog << "current coroutine is main coroutine";
        return;
    }
    Coroutine* co = t_cur_coroutine.get();
    t_cur_coroutine = t_main_coroutine;
    t_cur_run_time = NULL;
    coctx_swap(&(co->m_coctx), &(t_main_coroutine->m_coctx));
    // DebugLog << "swap back";
}

/********
form main coroutine switch to target coroutine
********/
void Coroutine::Resume(Coroutine::ptr co) {
    if (t_cur_coroutine != t_main_coroutine) {
        // ErrorLog << "swap error, current coroutine must be main coroutine";
        return;
    }

    if (!t_main_coroutine) {
        // ErrorLog << "main coroutine is nullptr";
        return;
    }
    if (!co || !co->m_can_resume) {
        // ErrorLog << "pending coroutine is nullptr or can_resume is false";
        return;
    }

    if (t_cur_coroutine == co) {
        // DebugLog << "current coroutine is pending cor, need't swap";
        return;
    }

    t_cur_coroutine = co;
    // t_cur_run_time = co->getRunTime();

    coctx_swap(&(t_main_coroutine->m_coctx), &(co->m_coctx));
    // DebugLog << "swap back";

}



}