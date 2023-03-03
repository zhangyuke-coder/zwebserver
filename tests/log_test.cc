#include "src/base/log/logging.h"

using namespace zws;
int main() {
    Logger::ptr g_logger = LOG_ROOT(); // 默认INFO级别
    LOG_FATAL(g_logger) << "fatal msg";
    LOG_ERROR(g_logger) << "err msg";
    LOG_INFO(g_logger) << "info msg";
    LOG_DEBUG(g_logger) << "debug msg";

    LOG_FMT_FATAL(g_logger, "fatal %s:%d", __FILE__, __LINE__);
    LOG_FMT_ERROR(g_logger, "err %s:%d", __FILE__, __LINE__);
    LOG_FMT_INFO(g_logger, "info %s:%d", __FILE__, __LINE__);
    LOG_FMT_DEBUG(g_logger, "debug %s:%d", __FILE__, __LINE__);


    FileLogAppender::ptr fileAppender(new zws::FileLogAppender("./log.txt"));
    g_logger->addAppender(fileAppender);
    LOG_FATAL(g_logger) << "fatal msg";
    LOG_ERROR(g_logger) << "err msg";
    LOG_INFO(g_logger) << "info msg"; // 不打印
    LOG_DEBUG(g_logger) << "debug msg"; // 不打印
}
