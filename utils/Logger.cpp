#include "Logger.h"
#include <stdarg.h>
#include <stdexcept>
#include <stdio.h>
#include <string.h>
#include <time.h>

using namespace utils;

const char *Logger::s_level[LEVEL_COUNT] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

Logger::Logger() : m_level(Logger::DEBUG), m_max(0), m_len(0) {
}

Logger::~Logger() {
}

Logger &Logger::get_instance() {
    static Logger instance;
    return instance;
}

void Logger::open(const std::string &filename) {
    m_filename = filename;
    m_fout.open(filename, std::ios::app);
    if (m_fout.fail()) {
        throw std::logic_error("open file failed " + filename);
    }
    m_fout.seekp(0, std::ios::end);
    m_len = m_fout.tellp();
}

void Logger::close() {
    m_fout.close();
}

void Logger::log(Level level, const char *file, int line, const char *format, ...) {
    if (level > m_level) {
        return;
    }
    if (m_fout.fail()) {
        throw std::logic_error("open file failed: " + m_filename);
    }
    time_t ticks = time(nullptr);
    struct tm *ptm = localtime(&ticks);
    char timestamp[32];
    memset(timestamp, 0, sizeof(timestamp));
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", ptm);
    const char *ftm = "%s %s %s:%d ";
    int size = snprintf(nullptr, 0, ftm, timestamp, s_level[level], file, line);
    if (size > 0) {
        char *buffer = new char[size + 1];
        // memset(buffer, 0, size + 1);
        snprintf(buffer, size + 1, ftm, timestamp, s_level[level], file, line);
        buffer[size] = 0;
        // std::cout << buffer << std::endl;
        m_fout << buffer;
        m_len += size;
        delete[] buffer;
    }
    va_list arg_ptr;
    va_start(arg_ptr, format);
    size = vsnprintf(nullptr, 0, format, arg_ptr);
    va_end(arg_ptr);
    if (size > 0) {
        char *content = new char[size + 1];
        va_start(arg_ptr, format);
        size = vsnprintf(content, size + 1, format, arg_ptr);
        va_end(arg_ptr);
        content[size] = 0;
        // std::cout << content << std::endl;
        m_fout << content;
        m_len += size;
        delete[] content;
    }
    m_fout << "\n";
    m_fout.flush();
    if (m_len >= m_max && m_max > 0) {
        rotate();
    }
}

void Logger::set_level(Level level) {
    m_level = level;
}

void Logger::set_max(int bytes) {
    m_max = bytes;
}

void Logger::rotate() {
    close();
    time_t ticks = time(nullptr);
    struct tm *ptm = localtime(&ticks);
    char timestamp[32];
    memset(timestamp, 0, sizeof(timestamp));
    strftime(timestamp, sizeof(timestamp), ".%Y-%m-%d_%H:%M:%S", ptm);
    std::string filename = m_filename + timestamp;
    // 备份
    if (rename(m_filename.c_str(), filename.c_str()) != 0) {
        throw std::logic_error("rename log file failed: " + std::string(strerror(errno)));
    };
    open(m_filename);
}