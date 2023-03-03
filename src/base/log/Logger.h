#pragma once
#include <memory>
#include "../mutex.h"
#include "LogLevel.h"
#include "LogAppend.h"
namespace zws {
    
/**
 * @brief 日志器类
 * @note 日志器类不带root logger
 */
class Logger{
public:
    typedef std::shared_ptr<Logger> ptr;
    typedef Spinlock MutexType;

    /**
     * @brief 构造函数
     * @param[in] name 日志器名称 
     */
    Logger(const std::string &name="default");

    /**
     * @brief 获取日志器名称
     */
    const std::string &getName() const { return m_name; }

    /**
     * @brief 获取创建时间
     */
    const uint64_t &getCreateTime() const { return m_createTime; }

    /**
     * @brief 设置日志级别
     */
    void setLevel(LogLevel::Level level) { m_level = level; }

    /**
     * @brief 获取日志级别
     */
    LogLevel::Level getLevel() const { return m_level; }

    /**
     * @brief 添加LogAppender
     */
    void addAppender(LogAppender::ptr appender);

    /**
     * @brief 删除LogAppender
     */
    void delAppender(LogAppender::ptr appender);

    /**
     * @brief 清空LogAppender
     */
    void clearAppenders();

    /**
     * @brief 写日志
     */
    void log(LogEvent::ptr event);

private:
    /// Mutex
    MutexType m_mutex;
    /// 日志器名称
    std::string m_name;
    /// 日志器等级
    LogLevel::Level m_level;
    /// LogAppender集合
    std::list<LogAppender::ptr> m_appenders;
    /// 创建时间（毫秒）
    uint64_t m_createTime;
};

}