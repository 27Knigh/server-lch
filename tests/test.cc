#include <iostream>
#include "../lch/log.h"

int main() {
    std::cout << "test" << '\n';
    lch::Logger::ptr logger(new lch::Logger);
    logger->addAppender(lch::LogAppender::ptr(new lch::StdoutLogAppender));

    lch::LogEvent::ptr event(new lch::LogEvent(__FILE__, __LINE__, 0, 1, 2, time(0)));
    event->getSS() << "Hellow world!";

    logger->log(lch::LogLevel::DEBUG, event);
    std::cout << "Hello world!" << std::endl;
    return 0;
}

