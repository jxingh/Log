#include "Logger.h"
using namespace utils;

int main() {
    // 设置日志存储位置
    Logger::get_instance().open("./test.log");
    // 设置日志级别
    Logger::get_instance().set_level(Logger::WARN);
    // 设置日志最大容量（字节）
    Logger::get_instance().set_max(1024);
    // Logger::get_instance().log(Logger::DEBUG, __FILE__, __LINE__, "hello world");
    // Logger::get_instance().log(Logger::DEBUG, __FILE__, __LINE__, "name is %s, age is %d", "Jack", 24);

    debug("hello world");
    debug("name is %s, age is %d", "Jack", 24);
    info("info message");
    warn("warn message");
    error("error message");
    fatal("fatal message");

    return 0;
}