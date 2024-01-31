#ifndef __LCH_LOG_H__
#define __LCH_LOG_H__
#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <functional>
#include <time.h>
#include <stdarg.h>

#include "./singleton.h"

//这条宏是为提供日志器的简便使用方式
#define LCH_LOG_LEVEL(logger, level) \
    if (logger->getLevel() <= level) \
        lch::LogEventWrap(lch::LogEvent::ptr(new lch::LogEvent(logger, level, __FILE__, __LINE__, 0,\
            lch::GetThreadId(), \
            lch::GetFiberId(), time(0)))).getSS()
//输出日志的方法：LCH_LOG_XX(logger) << content; 即可输出对应级别为xx的日志 
#define LCH_LOG_DEBUG(logger) LCH_LOG_LEVEL(logger, lch::LogLevel::DEBUG)
#define LCH_LOG_INFO(logger) LCH_LOG_LEVEL(logger, lch::LogLevel::INFO)
#define LCH_LOG_WARN(logger) LCH_LOG_LEVEL(logger, lch::LogLevel::WARN)
#define LCH_LOG_ERROR(logger) LCH_LOG_LEVEL(logger, lch::LogLevel::ERROR)
#define LCH_LOG_FATAL(logger) LCH_LOG_LEVEL(logger, lch::LogLevel::FATAL)


#define LCH_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    if (logger->getLevel() <= level) \
        lch::LogEventWrap(lch::LogEvent::ptr(new lch::LogEvent(logger, level, \
            __FILE__, __LINE__, 0, lch::GetThreadId(), \
            lch::GetFiberId(), time(0)))).getEvent()->format(fmt, __VA_ARGS__)

#define LCH_LOG_FMT_DEBUG(logger, fmt, ...) LCH_LOG_FMT_LEVEL(logger, lch::LogLevel::DEBUG, fmt, __VA_ARGS__)
#define LCH_LOG_FMT_INFO(logger, fmt, ...) LCH_LOG_FMT_LEVEL(logger, lch::LogLevel::INFO, fmt, __VA_ARGS__)
#define LCH_LOG_FMT_WARN(logger, fmt, ...) LCH_LOG_FMT_LEVEL(logger, lch::LogLevel::WARN, fmt, __VA_ARGS__)
#define LCH_LOG_FMT_ERROR(logger, fmt, ...) LCH_LOG_FMT_LEVEL(logger, lch::LogLevel::ERROR, fmt, __VA_ARGS__)
#define LCH_LOG_FMT_FATAL(logger, fmt, ...) LCH_LOG_FMT_LEVEL(logger, lch::LogLevel::FATAL, fmt, __VA_ARGS__)

namespace lch {

class Logger;

//日志级别
class LogLevel {
public:
    enum Level {
        UNKNOW = 0,
        DEBUG = 1,
        INFO = 2,
        WARN,
        ERROR,
        FATAL
    };

    static const char* ToString(LogLevel::Level level);
};


//日志事件 每条日志信息都是一个LogEvent对象    
class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level,const char* file, int32_t line, uint32_t elapse
    , uint32_t threadId, uint32_t fiberId, uint64_t time);

    ~LogEvent();

    const char* getFile() const {return m_file;}
    int32_t getLine() const {return m_line;}
    uint32_t getElapse() const {return m_elapse;}
    uint32_t getThreadId() const {return m_threadId;}
    uint32_t getFiberId() const {return m_fiberId;}
    std::uint64_t getTime() const {return m_time;}
    std::string getContent() const {return m_ss.str();}
    std::shared_ptr<Logger> getLogger() const {return m_logger;}
    LogLevel::Level getLevel() const {return m_level;}
    
    std::stringstream& getSS() {return m_ss;}
    void format(const char* fmt, ...);
    void format(const char* fmt, va_list al);
private:
    const char* m_file = nullptr;  //文件名
    int32_t m_line = 0;            //行号
    uint32_t m_elapse = 0;         //程序启动开始到现在的毫秒数
    uint32_t m_threadId = 0;       //线程Id
    uint32_t m_fiberId = 0;        //协程Id
    uint64_t m_time = 0;           //时间戳
    std::stringstream m_ss;

    std::shared_ptr<Logger> m_logger;
    LogLevel::Level m_level;
};

class LogEventWrap {
public:
    LogEventWrap(LogEvent::ptr e);
    ~LogEventWrap();
    std::stringstream& getSS();
    LogEvent::ptr getEvent() {return m_event;}
private:
    LogEvent::ptr m_event;
};




class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    LogFormatter(const std::string& pattern);

    //"%t   %thread_id %m%n"
    std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
public:
    class FormatItem {
    public:
        typedef std::shared_ptr<FormatItem> ptr;
        FormatItem(const std::string& fmt = "") {}
        virtual ~FormatItem() {}
        virtual void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::Level level, LogEvent::ptr event) = 0;
    };
    void init();
private:
    std::string m_pattern;
    std::vector<FormatItem::ptr> m_items;
};

//日志输出地
class LogAppender {
public:
    typedef std::shared_ptr<LogAppender> ptr;
    virtual ~LogAppender() {}

    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
    void setFormatter(LogFormatter::ptr val) { m_formatter = val; }
    LogFormatter::ptr getFormatter() const { return m_formatter; }

    LogLevel::Level getLevel() const { return m_level; }
    void setLevel(LogLevel::Level val) { m_level = val; }
protected:
    LogLevel::Level m_level = LogLevel::DEBUG;
    LogFormatter::ptr m_formatter;
};

//日志器
class Logger : public std::enable_shared_from_this<Logger> {
public:
    typedef std::shared_ptr<Logger>  ptr;
    Logger(const std::string& name = "root");
    void log(LogLevel::Level level, LogEvent::ptr event);

    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);
    LogLevel::Level getLevel() const {return m_level;}
    void setLevel(LogLevel::Level level) {m_level = level;}

    const std::string& getName() const { return m_name; }
private:
    std::string m_name;                    //日志名称
    LogLevel::Level m_level;               //日志级别
    std::list<LogAppender::ptr> m_appender;//appender集合
    LogFormatter::ptr m_formatter;
};

//输出到控制台的Appender
class StdoutLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    ~StdoutLogAppender() {}
    void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
};

//输出到文件的Appender
class FileLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const std::string& filename);
    ~FileLogAppender() {}
    void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
    
    //重新打开文件，成功返回true，失败返回false
    bool reopen();
private:
    std::string m_filename;
    std::ofstream m_filestream;
};


class LoggerManager {
public:
    LoggerManager();
    Logger::ptr getLogger(const std::string& name);

    void init();
private:
    std::map<std::string, Logger::ptr> m_loggers;
    Logger::ptr m_root;
};

typedef lch::Singleton<LoggerManager> LoggerMgr;

}

#endif
