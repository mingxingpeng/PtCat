//
// Created by Administrator on 2025/3/12.
//

/*
日志库核心要素
1. 日志级别 (Log Levels)
必须支持多种日志级别：TRACE, DEBUG, INFO, WARN, ERROR, FATAL

允许运行时动态调整日志级别

2. 输出目标 (Sinks)
支持多种输出目标：控制台、文件、网络、系统日志等

允许多个输出目标同时使用

3. 格式化 (Formatting)
可自定义的日志格式

支持时间戳、线程ID、文件名、行号等上下文信息

4. 性能考虑
尽量减少日志对主程序性能的影响

异步日志写入选项

避免日志I/O阻塞主线程

5. 线程安全
多线程环境下安全写入

适当的锁机制或无锁设计

6. 易用性
简单的API接口

流式输出或类似printf的格式化

宏定义简化调用

7. 扩展性
易于添加新的日志格式

易于添加新的输出目标

支持过滤器
 */
#ifndef PLOGWIRTER_H
#define PLOGWIRTER_H

//针对 windows 下静态库动态库不一致需要这样操作
/*#ifdef _WIN32
    #ifdef STATIC_LIB//对于 windows 静态库，也不需要指定
        #define DLL_API
    #else//对于 windows 动态库，如果是对应库文件，需要指定 __declspec(dllexport)，非库文件指定 __declspec(dllimport)
        #ifdef PLogWriter_EXPORTS//调用 add_libraries 才会有这个 xxx_EXPORTS 这个宏生成,单个单个是生成 PThreadPool 这个库
            #define DLL_API __declspec(dllexport)//windows 系统下对于动态库，需要指定 __declspec(dllexport)，不然会导致动态库 .lib 函数地址文件生成不了
        #else
            #ifdef PtCat_EXPORTS//如果是集体生成，会生成 PtCat 这个库
                #define DLL_API __declspec(dllexport)//windows 系统下对于动态库，需要指定 __declspec(dllexport)，不然会导致动态库 .lib 函数地址文件生成不了
            #else
                #define DLL_API __declspec(dllimport)
            #endif
        #endif
    #endif
#else//linux 不需要
    #define DLL_API
#endif*/

#include "common/common.h"

class DLL_API PLogWriter{
public:
    PLogWriter();
};

#endif //PLOGWIRTER_H
