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


namespace lch {

class Logger;

//日志事件 每条日志信息都是一个LogEvent对象    
class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent(const char* file, int32_t line, uint32_t elapse
    , uint32_t threadId, uint32_t fiberId, uint64_t time);

    const char* getFile() const {return m_file;}
    int32_t getLine() const {return m_line;}
    uint32_t getElapse() const {return m_elapse;}
    uint32_t getThreadId() const {return m_threadId;}
    uint32_t getFiberId() const {return m_fiberId;}
    std::uint64_t getTime() const {return m_time;}
    std::string getContent() const {return m_ss.str();}

    std::stringstream& getSS() {return m_ss;}

private:
    const char* m_file = nullptr;  //文件名
    int32_t m_line = 0;            //行号
    uint32_t m_elapse = 0;         //程序启动开始到现在的毫秒数
    uint32_t m_threadId = 0;       //线程Id
    uint32_t m_fiberId = 0;        //协程Id
    uint64_t m_time = 0;           //时间戳
    std::stringstream m_ss;
};

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

}

#endif
