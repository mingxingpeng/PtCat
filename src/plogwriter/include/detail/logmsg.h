//
// Created by 27852 on 2025/7/10.
//

#ifndef LOGMSG_H
#define LOGMSG_H

/*
日志的基本信息
*/

#include "core/core.h"
#include <chrono>

namespace ptcat {
    namespace plog {
        struct LogMsg {
            std::string name_;
            LogLevel log_level_{ALL};//默认定义为全局日志
            std::chrono::system_clock::time_point timestamp_{std::chrono::system_clock::now()};
            std::string msg_;//日志信息

            SourceInfo source_info_;//源文件所属文件名，函数名，行数

            LogMsg() = default;
            ~LogMsg() = default;
            LogMsg(std::string msg);
            LogMsg(std::chrono::system_clock::time_point timestamp, std::string msg);
            LogMsg(LogLevel log_level, std::chrono::system_clock::time_point timestamp, std::string msg);
            LogMsg(std::string name, LogLevel log_level, std::chrono::system_clock::time_point timestamp, std::string msg);
            LogMsg(SourceInfo source_info, std::string name, LogLevel log_level, std::chrono::system_clock::time_point timestamp, std::string msg);

            LogMsg(const LogMsg& log_msg) = default;
            LogMsg& operator=(const LogMsg& log_msg) = default;


        };
    }
}





#endif //LOGMSG_H
