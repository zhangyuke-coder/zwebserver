#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include "LogFormatter.h"
#include "../mutex.h"
namespace zws
{
    
/**
 * @brief 日志输出地，虚基类，用于派生出不同的LogAppender
 * @details 参考log4cpp，Appender自带一个默认的LogFormatter，以控件默认输出格式
 */
class LogAppender {
public:
    typedef std::shared_ptr<LogAppender> ptr;
    typedef Spinlock MutexType;

    /**
     * @brief 构造函数
     * @param[in] default_formatter 默认日志格式器
     */
    LogAppender(LogFormatter::ptr default_formatter);
    
    /**
     * @brief 析构函数
     */
    virtual ~LogAppender() {}

    /**
     * @brief 设置日志格式器
     */
    void setFormatter(LogFormatter::ptr val);

    /**
     * @brief 获取日志格式器
     */
    LogFormatter::ptr getFormatter();

    /**
     * @brief 写入日志
     */
    virtual void log(LogEvent::ptr event) = 0;
    /**
     * @brief 将日志输出目标的配置转成YAML String
     */
    virtual std::string toYamlString() = 0;

protected:
    /// Mutex
    MutexType m_mutex;
    /// 日志格式器
    LogFormatter::ptr m_formatter;
    /// 默认日志格式器
    LogFormatter::ptr m_defaultFormatter;
};

/**
 * @brief 输出到控制台的Appender
 */
class StdoutLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;

    /**
     * @brief 构造函数
     */
    StdoutLogAppender();

    /**
     * @brief 写入日志
     */
    void log(LogEvent::ptr event) override;
     /**
     * @brief 将日志输出目标的配置转成YAML String
     */
    std::string toYamlString() override;
};

/**
 * @brief 输出到文件
 */
class FileLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr;

    /**
     * @brief 构造函数
     * @param[] file 日志文件路径
     */
    FileLogAppender(const std::string &file);

    /**
     * @brief 写日志
     */
    void log(LogEvent::ptr event) override;

    /**
     * @brief 重新打开日志文件
     * @return 成功返回true
     */
    bool reopen();
    /**
     * @brief 将日志输出目标的配置转成YAML String
     */
    std::string toYamlString() override;

private:
    /// 文件路径
    std::string m_filename;
    /// 文件流
    std::ofstream m_filestream;
    /// 上次重打打开时间
    uint64_t m_lastTime = 0;
        /// 文件打开错误标识
    bool m_reopenError = false;
};
} // namespace zws
