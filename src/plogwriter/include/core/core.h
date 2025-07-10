//
// Created by 27852 on 2025/7/10.
//

#ifndef CORE_H
#define CORE_H

#include <string>

namespace ptcat {
    namespace plog {
        enum LogLevel : int{
            //往下从小往大，输出选中等级以及比他大的
            ALL = 0,//输出全部
            TARCE = 1,//杂项信息
            DEBUG = 2,//测试信息
            INFO = 3,//正常信息
            WARN = 4,//警告信息
            ERROR = 5,//错误信息
            FATAL = 6,//致命错误
        };

        //使用宏定义来对枚举进行字符串转换以及还原
#define PTCAT_LOG_LEVEL_ALL      "all"
#define PTCAT_LOG_LEVEL_TARCE    "tarce"
#define PTCAT_LOG_LEVEL_DEBUG    "debug"
#define PTCAT_LOG_LEVEL_INFO     "info"
#define PTCAT_LOG_LEVEL_WARN     "warn"
#define PTCAT_LOG_LEVEL_ERROR    "error"
#define PTCAT_LOG_LEVEL_FATAL    "fatal"

#if !defined(PTCAT_LOG_LEVEL_STRS)
    #define PTCAT_LOG_LEVEL_STRS      \
        {                            \
            PTCAT_LOG_LEVEL_ALL,      \
            PTCAT_LOG_LEVEL_TARCE,    \
            PTCAT_LOG_LEVEL_DEBUG,    \
            PTCAT_LOG_LEVEL_INFO,     \
            PTCAT_LOG_LEVEL_WARN,     \
            PTCAT_LOG_LEVEL_ERROR,    \
            PTCAT_LOG_LEVEL_FATAL     \
        }
#endif

        static const std::string log_levels_[] PTCAT_LOG_LEVEL_STRS;//存储日志等级枚举的值

        ///日志等级枚举转换为字符串
        inline const std::string& ToString(const LogLevel& level) {
            return log_levels_[level];//level 转换为数字索引
        }

        ///将对应字符串转换为枚举，如果没有则输出全部
        inline const LogLevel FromString(const std::string& level) {
            auto it = std::find(std::begin(log_levels_), std::end(log_levels_), level);
            if (it == std::end(log_levels_)) {
                return ALL;
            }else {
                return static_cast<LogLevel>(std::distance(std::begin(log_levels_), it));
            }
        }

        //日志记录方式
        enum WriteMethod : int{
            DIRECT_WRITE = 0,//直接写入
            CACHE_WRITE = 1,//缓存写入
            MIXED_WRITE = 2//混合写入,error 以上的直接写入，其他的缓存再写入
        };

        //日志所属源文件信息，也可以是头文件，命名而已，根据实际情况来使用
        struct SourceInfo {
            //源文件文件名
            const char* file_name_{nullptr};//文件名
            const char* func_name_{nullptr};//当前函数名称
            const int line_{-1};//当前代码所属行号

            SourceInfo() = default;
            ~SourceInfo() = default;
            SourceInfo(const char* func_name) : file_name_{nullptr}, func_name_{func_name}, line_{-1} {};
            SourceInfo( const char* func_name, const int line) : file_name_{nullptr}, func_name_{func_name}, line_{line} {};
            SourceInfo(const char* file_name, const char* func_name, const int line) : file_name_{file_name}, func_name_{func_name}, line_{line} {};

        };


    }
}
#endif //CORE_H
