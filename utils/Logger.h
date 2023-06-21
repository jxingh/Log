#ifndef _LOGGER_H
#define _LOGGER_H

#include <fstream>
#include <iostream>
#include <string>

namespace utils {

#define debug(format, ...) \
    Logger::get_instance().log(Logger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define info(format, ...) \
    Logger::get_instance().log(Logger::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define warn(format, ...) \
    Logger::get_instance().log(Logger::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define error(format, ...) \
    Logger::get_instance().log(Logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define fatal(format, ...) \
    Logger::get_instance().log(Logger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)

// 采用单例模式
class Logger {
public:
    enum Level {
        DEBUG = 0,
        INFO,
        WARN,
        ERROR,
        FATAL,
        LEVEL_COUNT
    };
    static Logger &get_instance();
    void open(const std::string &filename);
    void close();
    void log(Level level, const char *file, int line, const char *format, ...);
    void set_level(Level level);
    void set_max(int bytes);

private:
    Logger();
    ~Logger();
    Logger(const Logger &) = delete;
    Logger(Logger &&) = delete;
    const Logger &operator=(const Logger &) = delete;

    void rotate();

private:
    std::string m_filename;
    std::ofstream m_fout;
    Level m_level;
    int m_max;
    int m_len;
    static const char *s_level[LEVEL_COUNT];
};

}

#endif