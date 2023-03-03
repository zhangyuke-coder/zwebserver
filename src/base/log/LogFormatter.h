#pragma once
#include <vector>
#include "LogEvent.h"
namespace zws {
    
/**
 * @brief 日志格式化
 */
class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;

    /**
     * @brief 构造函数
     * @param[in] pattern 格式模板，参考sylar与log4cpp
     * @details 模板参数说明：
     * - %%m 消息
     * - %%p 日志级别
     * - %%c 日志器名称
     * - %%d 日期时间，后面可跟一对括号指定时间格式，比如%%d{%%Y-%%m-%%d %%H:%%M:%%S}，这里的格式字符与C语言strftime一致
     * - %%r 该日志器创建后的累计运行毫秒数
     * - %%f 文件名
     * - %%l 行号
     * - %%t 线程id
     * - %%F 协程id
     * - %%N 线程名称
     * - %%% 百分号
     * - %%T 制表符
     * - %%n 换行
     * 
     * 默认格式：%%d{%%Y-%%m-%%d %%H:%%M:%%S}%%T%%t%%T%%N%%T%%F%%T[%%p]%%T[%%c]%%T%%f:%%l%%T%%m%%n
     * 
     * 默认格式描述：年-月-日 时:分:秒 [累计运行毫秒数] \\t 线程id \\t 线程名称 \\t 协程id \\t [日志级别] \\t [日志器名称] \\t 文件名:行号 \\t 日志消息 换行符
     */
    LogFormatter(const std::string &pattern = "%d{%Y-%m-%d %H:%M:%S} [%rms]%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n");

    /**
     * @brief 初始化，解析格式模板，提取模板项
     */
    void init();

    /**
     * @brief 模板解析是否出错
     */
    bool isError() const { return m_error; }

    /**
     * @brief 对日志事件进行格式化，返回格式化日志文本
     * @param[in] event 日志事件
     * @return 格式化日志字符串
     */
    std::string format(LogEvent::ptr event);

    /**
     * @brief 对日志事件进行格式化，返回格式化日志流
     * @param[in] event 日志事件
     * @param[in] os 日志输出流
     * @return 格式化日志流
     */
    std::ostream &format(std::ostream &os, LogEvent::ptr event);

public:
    /**
     * @brief 日志内容格式化项，虚基类，用于派生出不同的格式化项
     */
    class FormatItem {
    public:
        typedef std::shared_ptr<FormatItem> ptr;
        
        /**
         * @brief 析构函数
         */
        virtual ~FormatItem() {}

        /**
         * @brief 格式化日志事件
         */
        virtual void format(std::ostream &os, LogEvent::ptr event) = 0;
    };

private:
    /// 日志格式模板
    std::string m_pattern;
    /// 解析后的格式模板数组
    std::vector<FormatItem::ptr> m_items;
    /// 是否出错
    bool m_error = false;
};
}