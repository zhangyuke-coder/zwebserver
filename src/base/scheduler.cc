<<<<<<< HEAD
#include "scheduler.h"
#include "src/base/macro.h"
using namespace zws;

=======
/**
 * @file scheduler.cc
 * @brief 协程调度器实现
 * @version 0.1
 * @date 2021-06-15
 */
#include "scheduler.h"
#include "macro.h"
#include "log/logging.h"
namespace zws {
>>>>>>> 7e0298e329ef53ccd53862ed5d4e1a64a8aa6cb2

static Logger::ptr g_logger = LOG_NAME("system");

/// 当前线程的调度器，同一个调度器下的所有线程共享同一个实例
static thread_local Scheduler *t_scheduler = nullptr;
/// 当前线程的调度协程，每个线程都独有一份
<<<<<<< HEAD
static thread_local Fiber *t_scheduler_fiber = nullptr;
=======
static thread_local Coroutine *t_scheduler_fiber = nullptr;
>>>>>>> 7e0298e329ef53ccd53862ed5d4e1a64a8aa6cb2

Scheduler::Scheduler(size_t threads, bool use_caller, const std::string &name) {
    ASSERT(threads > 0);

    m_useCaller = use_caller;
    m_name      = name;

    if (use_caller) {
        --threads;
<<<<<<< HEAD
        Fiber::GetThis();
        ASSERT(GetThis() == nullptr);
=======
        // sylar::Fiber::GetThis();
        // ASSERT(GetThis() == nullptr);
>>>>>>> 7e0298e329ef53ccd53862ed5d4e1a64a8aa6cb2
        t_scheduler = this;

        /**
         * caller线程的主协程不会被线程的调度协程run进行调度，而且，线程的调度协程停止时，应该返回caller线程的主协程
         * 在user caller情况下，把caller线程的主协程暂时保存起来，等调度协程结束时，再resume caller协程
         */
<<<<<<< HEAD
        m_rootFiber.reset(new Fiber(std::bind(&Scheduler::run, this), 0, false));

        Thread::SetName(m_name);
        t_scheduler_fiber = m_rootFiber.get();
        m_rootThread      = GetThreadId();
=======
        int stack_size = 128 * 1024;
        char* sp2 = reinterpret_cast<char*>(malloc(stack_size));
        // m_rootFiber.reset(new Coroutine(std::bind(&Scheduler::run, this), 0, false));
        // stack_size, 
        m_rootFiber.reset(new Coroutine(stack_size, sp2, std::bind(&Scheduler::run, this)));
        Thread::SetName(m_name);
        t_scheduler_fiber = m_rootFiber.get();
        m_rootThread      = zws::GetThreadId();
>>>>>>> 7e0298e329ef53ccd53862ed5d4e1a64a8aa6cb2
        m_threadIds.push_back(m_rootThread);
    } else {
        m_rootThread = -1;
    }
    m_threadCount = threads;
}

Scheduler *Scheduler::GetThis() { 
    return t_scheduler; 
}

<<<<<<< HEAD
Fiber *Scheduler::GetMainFiber() { 
=======
Coroutine *Scheduler::GetMainCoroutine() { 
>>>>>>> 7e0298e329ef53ccd53862ed5d4e1a64a8aa6cb2
    return t_scheduler_fiber;
}

void Scheduler::setThis() {
    t_scheduler = this;
}

Scheduler::~Scheduler() {
    LOG_DEBUG(g_logger) << "Scheduler::~Scheduler()";
    ASSERT(m_stopping);
    if (GetThis() == this) {
        t_scheduler = nullptr;
    }
}

void Scheduler::start() {
    LOG_DEBUG(g_logger) << "start";
    MutexType::Lock lock(m_mutex);
    if (m_stopping) {
        LOG_ERROR(g_logger) << "Scheduler is stopped";
        return;
    }
    ASSERT(m_threads.empty());
    m_threads.resize(m_threadCount);
    for (size_t i = 0; i < m_threadCount; i++) {
        m_threads[i].reset(new Thread(std::bind(&Scheduler::run, this),
                                      m_name + "_" + std::to_string(i)));
        m_threadIds.push_back(m_threads[i]->getId());
    }
}

bool Scheduler::stopping() {
    MutexType::Lock lock(m_mutex);
    return m_stopping && m_tasks.empty() && m_activeThreadCount == 0;
}

void Scheduler::tickle() { 
    LOG_DEBUG(g_logger) << "ticlke"; 
}

void Scheduler::idle() {
    LOG_DEBUG(g_logger) << "idle";
    while (!stopping()) {
<<<<<<< HEAD
        Fiber::GetThis()->yield();
=======
        Coroutine::Yield();
>>>>>>> 7e0298e329ef53ccd53862ed5d4e1a64a8aa6cb2
    }
}

void Scheduler::stop() {
    LOG_DEBUG(g_logger) << "stop";
    if (stopping()) {
        return;
    }
    m_stopping = true;

    /// 如果use caller，那只能由caller线程发起stop
    if (m_useCaller) {
        ASSERT(GetThis() == this);
    } else {
        ASSERT(GetThis() != this);
    }

    for (size_t i = 0; i < m_threadCount; i++) {
        tickle();
    }

    if (m_rootFiber) {
        tickle();
    }

    /// 在use caller情况下，调度器协程结束时，应该返回caller协程
    if (m_rootFiber) {
<<<<<<< HEAD
        m_rootFiber->resume();
=======
        // m_rootFiber->resume();
        Coroutine::Resume(m_rootFiber);
>>>>>>> 7e0298e329ef53ccd53862ed5d4e1a64a8aa6cb2
        LOG_DEBUG(g_logger) << "m_rootFiber end";
    }

    std::vector<Thread::ptr> thrs;
    {
        MutexType::Lock lock(m_mutex);
        thrs.swap(m_threads);
    }
    for (auto &i : thrs) {
        i->join();
    }
}

void Scheduler::run() {
    LOG_DEBUG(g_logger) << "run";
    setThis();
<<<<<<< HEAD
    if (GetThreadId() != m_rootThread) {
        t_scheduler_fiber = Fiber::GetThis().get();
    }

    Fiber::ptr idle_fiber(new Fiber(std::bind(&Scheduler::idle, this)));
    Fiber::ptr cb_fiber;
=======
    if (zws::GetThreadId() != m_rootThread) {
        t_scheduler_fiber = Coroutine::GetCurrentCoroutine().get();
    }
    int stack_size = 128 * 1024;
    // char* sp = reinterpret_cast<char*>(malloc(stack_size));
    // cor = std::make_shared<zrpc::Coroutine>(stack_size, sp, fun_2);

    char* sp2 = reinterpret_cast<char*>(malloc(stack_size));

    Coroutine::ptr idle_fiber(new Coroutine(stack_size, sp2, std::bind(&Scheduler::idle, this)));
    Coroutine::ptr cb_fiber;
>>>>>>> 7e0298e329ef53ccd53862ed5d4e1a64a8aa6cb2

    ScheduleTask task;
    while (true) {
        task.reset();
        bool tickle_me = false; // 是否tickle其他线程进行任务调度
        {
            MutexType::Lock lock(m_mutex);
            auto it = m_tasks.begin();
            // 遍历所有调度任务
            while (it != m_tasks.end()) {
<<<<<<< HEAD
                if (it->thread != -1 && it->thread != GetThreadId()) {
=======
                if (it->thread != -1 && it->thread != zws::GetThreadId()) {
>>>>>>> 7e0298e329ef53ccd53862ed5d4e1a64a8aa6cb2
                    // 指定了调度线程，但不是在当前线程上调度，标记一下需要通知其他线程进行调度，然后跳过这个任务，继续下一个
                    ++it;
                    tickle_me = true;
                    continue;
                }

                // 找到一个未指定线程，或是指定了当前线程的任务
                ASSERT(it->fiber || it->cb);
                if (it->fiber) {
                    // 任务队列时的协程一定是READY状态，谁会把RUNNING或TERM状态的协程加入调度呢？
<<<<<<< HEAD
                    ASSERT(it->fiber->getState() == Fiber::READY);
=======
                    ASSERT(it->fiber->getState() == Coroutine::READY);
>>>>>>> 7e0298e329ef53ccd53862ed5d4e1a64a8aa6cb2
                }
                // 当前调度线程找到一个任务，准备开始调度，将其从任务队列中剔除，活动线程数加1
                task = *it;
                m_tasks.erase(it++);
                ++m_activeThreadCount;
                break;
            }
            // 当前线程拿完一个任务后，发现任务队列还有剩余，那么tickle一下其他线程
            tickle_me |= (it != m_tasks.end());
        }

        if (tickle_me) {
            tickle();
        }

        if (task.fiber) {
            // resume协程，resume返回时，协程要么执行完了，要么半路yield了，总之这个任务就算完成了，活跃线程数减一
<<<<<<< HEAD
            task.fiber->resume();
=======
            Coroutine::Resume(task.fiber);
>>>>>>> 7e0298e329ef53ccd53862ed5d4e1a64a8aa6cb2
            --m_activeThreadCount;
            task.reset();
        } else if (task.cb) {
            if (cb_fiber) {
                cb_fiber->reset(task.cb);
            } else {
<<<<<<< HEAD
                cb_fiber.reset(new Fiber(task.cb));
            }
            task.reset();
            cb_fiber->resume();
=======
                int stack_size = 128 * 1024;
                char* sp2 = reinterpret_cast<char*>(malloc(stack_size));
                cb_fiber.reset(new Coroutine(stack_size, sp2, std::bind(&Scheduler::idle, this)));
            }
            task.reset();
            Coroutine::Resume(cb_fiber);
>>>>>>> 7e0298e329ef53ccd53862ed5d4e1a64a8aa6cb2
            --m_activeThreadCount;
            cb_fiber.reset();
        } else {
            // 进到这个分支情况一定是任务队列空了，调度idle协程即可
<<<<<<< HEAD
            if (idle_fiber->getState() == Fiber::TERM) {
=======
            if (idle_fiber->getState() == Coroutine::TERM) {
>>>>>>> 7e0298e329ef53ccd53862ed5d4e1a64a8aa6cb2
                // 如果调度器没有调度任务，那么idle协程会不停地resume/yield，不会结束，如果idle协程结束了，那一定是调度器停止了
                LOG_DEBUG(g_logger) << "idle fiber term";
                break;
            }
            ++m_idleThreadCount;
<<<<<<< HEAD
            idle_fiber->resume();
=======
            Coroutine::Resume(idle_fiber);
>>>>>>> 7e0298e329ef53ccd53862ed5d4e1a64a8aa6cb2
            --m_idleThreadCount;
        }
    }
    LOG_DEBUG(g_logger) << "Scheduler::run() exit";
<<<<<<< HEAD
}
=======
}

} // end namespace sylar
>>>>>>> 7e0298e329ef53ccd53862ed5d4e1a64a8aa6cb2
