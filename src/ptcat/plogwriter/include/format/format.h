//
// Created by 27852 on 2025/7/10.
//

#ifndef FORMAT_H
#define FORMAT_H

#include "detail/logmsg.h"
/*
// %+ 使用默认格式
spdlog::set_pattern("%+");

// 自定义格式
spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^%l%$] [thread %t] %v");

// 单独设置logger格式
logger->set_pattern("%Y-%m-%d %H:%M:%S.%f [%l] %v");
 */
namespace ptcat {
    namespace plog {
        //文件格式基类
        class Format {

            //定义文件的格式总函数
            virtual std::string Fmt(const LogMsg& msg);

            //根据符号处理日志信息
            std::string HandleMsg(char flag, const LogMsg& ms);

            //根据 C++ 自带的时间格式来获取到对应的时间字符串
            std::string FormatTime(const std::chrono::system_clock::time_point& time, const char* fmt);
        };
    }
}

#endif //FORMAT_H

/*
class SimpleFormatter {
public:
    explicit SimpleFormatter(const std::string& pattern = "[%Y-%m-%d %H:%M:%S] [%l] %v")
        : pattern_(pattern) {}

    std::string format(const LogMsg& msg) {
        std::string result;
        size_t pos = 0;

        while (pos < pattern_.length()) {
            if (pattern_[pos] == '%' && pos + 1 < pattern_.length()) {
                // 处理格式标记
                char flag = pattern_[pos + 1];
                result += handle_flag(flag, msg);
                pos += 2;
            } else {
                // 普通文本直接追加
                result += pattern_[pos];
                pos += 1;
            }
        }

        return result;
    }

private:
    std::string handle_flag(char flag, const LogMsg& msg) {
        switch (flag) {
            case 'v': return msg.payload;       // 消息内容
            case 'l': return level_to_str(msg.level); // 日志级别
            case 'n': return msg.logger_name;   // 记录器名称
            case 'Y': return format_time(msg.time, "%Y"); // 年
            case 'm': return format_time(msg.time, "%m"); // 月
            case 'd': return format_time(msg.time, "%d"); // 日
            case 'H': return format_time(msg.time, "%H"); // 时
            case 'M': return format_time(msg.time, "%M"); // 分
            case 'S': return format_time(msg.time, "%S"); // 秒
            case 's': return msg.filename;      // 源文件
            case '#': return std::to_string(msg.line); // 行号
            case '!': return msg.funcname;      // 函数名
            default:  return std::string("%") + flag; // 未知标记
        }
    }

    std::string level_to_str(LogLevel level) {
        static const char* level_names[] = {"TRACE", "DEBUG", "INFO",
                                          "WARN", "ERROR", "CRITICAL"};
        return level_names[static_cast<int>(level)];
    }

    std::string format_time(std::chrono::system_clock::time_point tp,
                          const char* fmt) {
        auto t = std::chrono::system_clock::to_time_t(tp);
        tm tm;
        localtime_r(&t, &tm);

        char buf[64];
        strftime(buf, sizeof(buf), fmt, &tm);
        return buf;
    }

    std::string pattern_;
};
 */
