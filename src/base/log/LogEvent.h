#pragma once
#include <memory>
#include <sstream>
#include "LogLevel.h"
namespace zws {
/**
 * @brief 日志事件
 */
class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;

    /**
     * @brief 构造函数
     * @param[in] logger_name 日志器名称
     * @param[in] level 日志级别
     * @param[in] file 文件名
     * @param[in] line 行号
     * @param[in] elapse 从日志器创建开始到当前的累计运行毫秒
     * @param[in] thead_id 线程id
     * @param[in] fiber_id 协程id
     * @param[in] time UTC时间
     * @param[in] thread_name 线程名称
     */
    LogEvent(const std::string &logger_name, LogLevel::Level level, const char *file, int32_t line
        , int64_t elapse, uint32_t thread_id, uint64_t fiber_id, time_t time, const std::string &thread_name);

    /**
     * @brief 获取日志级别
     */
    LogLevel::Level getLevel() const { return m_level; }

    /**
     * @brief 获取日志内容
     */
    std::string getContent() const { return m_ss.str(); }

    /**
     * @brief 获取文件名
     */
    std::string getFile() const { return m_file; }

    /**
     * @brief 获取行号
     */
    int32_t getLine() const { return m_line; }

    /**
     * @brief 获取累计运行毫秒数
     */
    int64_t getElapse() const { return m_elapse; }

    /**
     * @brief 获取线程id
     */
    uint32_t getThreadId() const { return m_threadId; }

    /**
     * @brief 获取协程id
     */
    uint64_t getFiberId() const { return m_fiberId; }

    /**
     * @brief 返回时间戳
     */
    time_t getTime() const { return m_time; }

    /**
     * @brief 获取线程名称
     */
    const std::string &getThreadName() const { return m_threadName; }

    /**
     * @brief 获取内容字节流，用于流式写入日志
     */
    std::stringstream &getSS() { return m_ss; }

    /**
     * @brief 获取日志器名称
     */
    const std::string &getLoggerName() const { return m_loggerName; }

    /**
     * @brief C prinf风格写入日志
     */
    void printf(const char *fmt, ...);

    /**
     * @brief C vprintf风格写入日志
     */
    void vprintf(const char *fmt, va_list ap);

private:
    /// 日志级别
    LogLevel::Level m_level;
    /// 日志内容，使用stringstream存储，便于流式写入日志
    std::stringstream m_ss;
    /// 文件名
    const char *m_file = nullptr;
    /// 行号
    int32_t m_line = 0;
    /// 从日志器创建开始到当前的耗时
    int64_t m_elapse = 0;
    /// 线程id
    uint32_t m_threadId = 0;
    /// 协程id
    uint64_t m_fiberId = 0;
    /// UTC时间戳
    time_t m_time;
    /// 线程名称
    std::string m_threadName;
    /// 日志器名称
    std::string m_loggerName;
};
}
