#include "LogAppend.h"

namespace zws {
    
LogAppender::LogAppender(LogFormatter::ptr default_formatter)
    : m_defaultFormatter(default_formatter) {
}

void LogAppender::setFormatter(LogFormatter::ptr val) {
    MutexType::Lock lock(m_mutex);
    m_formatter = val;
}

LogFormatter::ptr LogAppender::getFormatter() {
    MutexType::Lock lock(m_mutex);
    return m_formatter;
}

StdoutLogAppender::StdoutLogAppender()
    : LogAppender(LogFormatter::ptr(new LogFormatter)) {
}

void StdoutLogAppender::log(LogEvent::ptr event) {
    if(m_formatter) {
        m_formatter->format(std::cout, event);
    } else {
        m_defaultFormatter->format(std::cout, event);
    }
}

FileLogAppender::FileLogAppender(const std::string &file)
    : LogAppender(LogFormatter::ptr(new LogFormatter)) {
    m_filename = file;
    reopen();
}

/**
 * 如果一个日志事件距离上次写日志超过3秒，那就重新打开一次日志文件
 */
void FileLogAppender::log(LogEvent::ptr event) {
    uint64_t now = event->getTime();
    if(now >= (m_lastTime + 3)) {
        reopen();
        m_lastTime = now;
    }
    MutexType::Lock lock(m_mutex);
    if(m_formatter) {
        if(!m_formatter->format(m_filestream, event)) {
            std::cout << "[ERROR] FileLogAppender::log() format error" << std::endl;
        }
    } else {
        if(!m_defaultFormatter->format(m_filestream, event)) {
            std::cout << "[ERROR] FileLogAppender::log() format error" << std::endl;
        }
    }
    
}

bool FileLogAppender::reopen() {
    MutexType::Lock lock(m_mutex);
    if(m_filestream) {
        m_filestream.close();
    }
    m_filestream.open(m_filename, std::ios::app);
    return !!m_filestream;
}
}