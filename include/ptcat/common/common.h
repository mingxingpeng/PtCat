//
// Created by Administrator on 2025/3/17.
//

/****
公共库

*/

#ifndef COMMON_H
#define COMMON_H

//***************************************************
//windows 静态库，动态库区分
//***************************************************
#ifdef _WIN32//针对 windows 下静态库动态库不一致需要这样操作
    #ifdef STATIC_LIB//对于 windows 静态库，也不需要指定
        #define DLL_API
    #else//对于 windows 动态库，如果是对应库文件，需要指定 __declspec(dllexport)，非库文件指定 __declspec(dllimport)
        #ifdef ptcat_EXPORTS//调用 add_libraries 才会有这个 xxx_EXPORTS 这个宏生成,单个单个是生成 PThreadPool 这个库
            #define DLL_API __declspec(dllexport)//windows 系统下对于动态库，需要指定 __declspec(dllexport)，不然会导致动态库 .lib 函数地址文件生成不了
        #elif PThreadPool_EXPORTS//线程池库
            #define DLL_API __declspec(dllexport)
        #elif PLogWriter_EXPORTS//日志库
            #define DLL_API __declspec(dllexport)
        #elif PDateTime_EXPORTS//时间库
            #define DLL_API __declspec(dllexport)
        #else
            #define DLL_API __declspec(dllimport)
        #endif
    #endif
#else//linux 不需要
    #define DLL_API
#endif


namespace ptcat {
    typedef long long llong;
    typedef unsigned int uint;
    typedef unsigned char uchar;

}


#endif //COMMON_H
