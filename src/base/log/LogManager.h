#pragma once
#include <map>
#include "Logger.h"
#include "../singleton.h"

namespace zws {
    /**
 * @brief 日志事件包装器，方便宏定义，内部包含日志事件和日志器
 */
class LogEventWrap{
public:
    /**
     * @brief 构造函数
     * @param[] logger 日志器 
     * @param[] event 日志事件
     */
    LogEventWrap(Logger::ptr logger, LogEvent::ptr event);

    /**
     * @brief 析构函数
     * @details 日志事件在析构时由日志器进行输出
     */
    ~LogEventWrap();

    /**
     * @brief 获取日志事件
     */
    LogEvent::ptr getLogEvent() const { return m_event; }

private:
    /// 日志器
    Logger::ptr m_logger;
    /// 日志事件
    LogEvent::ptr m_event;
};

/**
 * @brief 日志器管理类
 */
class LoggerManager{
public:
    typedef Spinlock MutexType;

    /**
     * @brief 构造函数
     */
    LoggerManager();

    /**
     * @brief 初始化，主要是结合配置模块实现日志模块初始化
     */
    void init();

    /**
     * @brief 获取指定名称的日志器
     */
    Logger::ptr getLogger(const std::string &name);

    /**
     * @brief 获取root日志器，等效于getLogger("root")
     */
    Logger::ptr getRoot() { return m_root; }

private:
    /// Mutex
    MutexType m_mutex;
    /// 日志器集合
    std::map<std::string, Logger::ptr> m_loggers;
    /// root日志器
    Logger::ptr m_root;
};

/// 日志器管理类单例
typedef zws::Singleton<LoggerManager> LoggerMgr;
}