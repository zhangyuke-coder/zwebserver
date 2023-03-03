#pragma once
#include <string>
namespace zws {

/**
 * @brief 日志级别
 */
class LogLevel {
public:
    /**
     * @brief 日志级别枚举，参考log4cpp
     */
    enum Level { 
        /// 致命情况，系统不可用
        FATAL  = 0,
        /// 高优先级情况，例如数据库系统崩溃
        ALERT  = 100,
        /// 严重错误，例如硬盘错误
        CRIT   = 200,
        /// 错误
        ERROR  = 300,
        /// 警告
        WARN   = 400,
        /// 正常但值得注意
        NOTICE = 500,
        /// 一般信息
        INFO   = 600,
        /// 调试信息
        DEBUG  = 700,
        /// 未设置
        NOTSET = 800,
    };

    /**
     * @brief 日志级别转字符串
     * @param[in] level 日志级别 
     * @return 字符串形式的日志级别
     */
    static const char *ToString(LogLevel::Level level);

    /**
     * @brief 字符串转日志级别
     * @param[in] str 字符串 
     * @return 日志级别
     * @note 不区分大小写
     */
    static LogLevel::Level FromString(const std::string &str);
};
}