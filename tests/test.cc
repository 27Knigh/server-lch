#include <iostream>
#include "../lch/log.h"
#include "../lch/util.h"

int main() {
    lch::Logger::ptr logger(new lch::Logger);
    logger->addAppender(lch::LogAppender::ptr(new lch::StdoutLogAppender));

    lch::FileLogAppender::ptr file_appender(new lch::FileLogAppender("./log.txt"));
    

    lch::LogFormatter::ptr fmt(new lch::LogFormatter("%d%T%p%T%m%n"));
    file_appender->setFormatter(fmt);
    file_appender->setLevel(lch::LogLevel::ERROR);
    logger->addAppender(file_appender);
    //lch::LogEvent::ptr event(new lch::LogEvent(__FILE__, __LINE__, 0, lch::GetThreadId(), lch::GetFiberId(), time(0)));
    //event->getSS() << "Hellow world!";

    //logger->log(lch::LogLevel::DEBUG, event);
    std::cout << "Hello world!" << std::endl;
    LCH_LOG_INFO(logger) << "test info";
    LCH_LOG_ERROR(logger) << "test macro error";

    LCH_LOG_FMT_ERROR(logger, "test macro fmt error %s", "aa");

    auto l = lch::LoggerMgr::GetInstance()->getLogger("xx");
    LCH_LOG_INFO(l) << "xxxx";
    return 0;
}

