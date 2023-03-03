#pragma once
#include "LogManager.h"
#include "LogEvent.h"
#include "LogLevel.h"
#include "Logger.h"
#include "LogAppend.h"
#include "../Util.h"
/**
 * @brief 获取root日志器
 */
#define LOG_ROOT() zws::LoggerMgr::GetInstance()->getRoot()

/**
 * @brief 获取指定名称的日志器
 */
#define LOG_NAME(name) zws::LoggerMgr::GetInstance()->getLogger(name)

/**
 * @brief 使用流式方式将日志级别level的日志写入到logger
 * @details 构造一个LogEventWrap对象，包裹包含日志器和日志事件，在对象析构时调用日志器写日志事件
 * @todo 协程id未实现，暂时写0
 */
#define LOG_LEVEL(logger , level) \
    if(level <= logger->getLevel()) \
        zws::LogEventWrap(logger, zws::LogEvent::ptr(new zws::LogEvent(logger->getName(), \
            level, __FILE__, __LINE__, zws::GetElapsed() - logger->getCreateTime(), \
            zws::GetThreadId(), zws::GetFiberId(), time(0), zws::GetThreadName()))).getLogEvent()->getSS()

#define LOG_FATAL(logger) LOG_LEVEL(logger, zws::LogLevel::FATAL)

#define LOG_ALERT(logger) LOG_LEVEL(logger, zws::LogLevel::ALERT)

#define LOG_CRIT(logger) LOG_LEVEL(logger, zws::LogLevel::CRIT)

#define LOG_ERROR(logger) LOG_LEVEL(logger, zws::LogLevel::ERROR)

#define LOG_WARN(logger) LOG_LEVEL(logger, zws::LogLevel::WARN)

#define LOG_NOTICE(logger) LOG_LEVEL(logger, zws::LogLevel::NOTICE)

#define LOG_INFO(logger) LOG_LEVEL(logger, zws::LogLevel::INFO)

#define LOG_DEBUG(logger) LOG_LEVEL(logger, zws::LogLevel::DEBUG)

/**
 * @brief 使用C printf方式将日志级别level的日志写入到logger
 * @details 构造一个LogEventWrap对象，包裹包含日志器和日志事件，在对象析构时调用日志器写日志事件
 * @todo 协程id未实现，暂时写0
 */
#define LOG_FMT_LEVEL(logger, level, fmt, ...) \
    if(level <= logger->getLevel()) \
        zws::LogEventWrap(logger, zws::LogEvent::ptr(new zws::LogEvent(logger->getName(), \
            level, __FILE__, __LINE__, zws::GetElapsed() - logger->getCreateTime(), \
            zws::GetThreadId(), zws::GetFiberId(), time(0), zws::GetThreadName()))).getLogEvent()->printf(fmt, __VA_ARGS__)

#define LOG_FMT_FATAL(logger, fmt, ...) LOG_FMT_LEVEL(logger, zws::LogLevel::FATAL, fmt, __VA_ARGS__)

#define LOG_FMT_ALERT(logger, fmt, ...) LOG_FMT_LEVEL(logger, zws::LogLevel::ALERT, fmt, __VA_ARGS__)

#define LOG_FMT_CRIT(logger, fmt, ...) LOG_FMT_LEVEL(logger, zws::LogLevel::CRIT, fmt, __VA_ARGS__)

#define LOG_FMT_ERROR(logger, fmt, ...) LOG_FMT_LEVEL(logger, zws::LogLevel::ERROR, fmt, __VA_ARGS__)

#define LOG_FMT_WARN(logger, fmt, ...) LOG_FMT_LEVEL(logger, zws::LogLevel::WARN, fmt, __VA_ARGS__)

#define LOG_FMT_NOTICE(logger, fmt, ...) LOG_FMT_LEVEL(logger, zws::LogLevel::NOTICE, fmt, __VA_ARGS__)

#define LOG_FMT_INFO(logger, fmt, ...) LOG_FMT_LEVEL(logger, zws::LogLevel::INFO, fmt, __VA_ARGS__)

#define LOG_FMT_DEBUG(logger, fmt, ...) LOG_FMT_LEVEL(logger, zws::LogLevel::DEBUG, fmt, __VA_ARGS__)
