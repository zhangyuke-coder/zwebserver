#pragma once 
#include "LogManager.h"
namespace zws {
    
LogEventWrap::LogEventWrap(Logger::ptr logger, LogEvent::ptr event)
    : m_logger(logger)
    , m_event(event) {
}

/**
 * @note LogEventWrap在析构时写日志
 */
LogEventWrap::~LogEventWrap() {
    m_logger->log(m_event);
}

LoggerManager::LoggerManager() {
    m_root.reset(new Logger("root"));
    m_root->addAppender(LogAppender::ptr(new StdoutLogAppender));
    m_loggers[m_root->getName()] = m_root;
    init();
}

/**
 * 如果指定名称的日志器未找到，那会就新创建一个，但是新创建的Logger是不带Appender的，
 * 需要手动添加Appender
 */
Logger::ptr LoggerManager::getLogger(const std::string &name) {
    MutexType::Lock lock(m_mutex);
    auto it = m_loggers.find(name);
    if(it != m_loggers.end()) {
        return it->second;
    }

    Logger::ptr logger(new Logger(name));
    m_loggers[name] = logger;
    return logger;
}

/**
 * @todo 实现从配置文件加载日志配置
 */
void LoggerManager::init() {
}
}