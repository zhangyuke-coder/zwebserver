#include "Logger.h"
#include "../Util.h"
namespace zws {
    
Logger::Logger(const std::string &name)
    : m_name(name)
    , m_level(LogLevel::INFO)
    , m_createTime(GetElapsed()) {
    }

void Logger::addAppender(LogAppender::ptr appender) {
    MutexType::Lock lock(m_mutex);
    m_appenders.push_back(appender);
}

void Logger::delAppender(LogAppender::ptr appender) {
    MutexType::Lock lock(m_mutex);
    for(auto it = m_appenders.begin(); it != m_appenders.end(); it++) {
        if(*it == appender) {
            m_appenders.erase(it);
            break;
        }
    }
}

void Logger::clearAppenders() {
    MutexType::Lock lock(m_mutex);
    m_appenders.clear();
}

/**
 * 调用Logger的所有appenders将日志写一遍，
 * Logger至少要有一个appender，否则没有输出
 */
void Logger::log(LogEvent::ptr event) {
    if(event->getLevel() <= m_level) {
        for(auto &i : m_appenders) {
            i->log(event);
        }
    }
}
}