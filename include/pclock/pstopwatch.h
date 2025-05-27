//
// Created by 27852 on 2025/5/14.
//

#ifndef PDATETIME_H
#define PDATETIME_H

//针对 windows 下静态库动态库不一致需要这样操作
// #ifdef _WIN32
//     #ifdef STATIC_LIB//对于 windows 静态库，也不需要指定
//         #define DLL_API
//     #else//对于 windows 动态库，如果是对应库文件，需要指定 __declspec(dllexport)，非库文件指定 __declspec(dllimport)
//         #ifdef PDateTime_EXPORTS//调用 add_libraries 才会有这个 xxx_EXPORTS 这个宏生成,单个单个是生成 PThreadPool 这个库
//             #define DLL_API __declspec(dllexport)//windows 系统下对于动态库，需要指定 __declspec(dllexport)，不然会导致动态库 .lib 函数地址文件生成不了
//         #else
//             #ifdef PtCat_EXPORTS//如果是集体生成，会生成 PtCat 这个库
//                 #define DLL_API __declspec(dllexport)//windows 系统下对于动态库，需要指定 __declspec(dllexport)，不然会导致动态库 .lib 函数地址文件生成不了
//             #else
//                 #define DLL_API __declspec(dllimport)
//             #endif
//         #endif
//     #endif
// #else//linux 不需要
//     #define DLL_API
// #endif

#include "chronograph.h"
#include <string>
#include <vector>

namespace ptcat {
    namespace clock {
        struct LapInfo {
            std::string name_;
            PLLONG time_;//elapsed time

            LapInfo(std::string name, PLLONG time) : name_(name), time_(time){}
        };

        /// <summary>
        /// 用于计时当前程序大致使用了多少秒
        /// </summary>
        class DLL_API StopWatch : public ChronoGraph{
        private:
            std::vector<LapInfo> laps_;//lap times

            PLLONG CalcElapsed(std::string name = "");

        public:
            //这里推荐初始化列表使用花括号，更现代，因为对于容器类型数据会更友好， vector{1, 2, 3}
            StopWatch();

            ~StopWatch();

            void Clear() {
                laps_.clear();
            }


            void LapStart(std::string name = "total");//start time

            void LapStop();//stop time

            PLLONG MicroLapTime(std::string name = "total");

            PLLONG MilliLapTime(std::string name = "total");

            PLLONG LapTime(std::string name = "total");

            PLLONG MicroTotalTime();//get total time

            PLLONG MilliTotalTime();//get total time

            PLLONG TotalTime();//get total time
        };
    }

}


#endif //PDATETIME_H
