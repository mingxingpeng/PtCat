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
#include <map>

namespace ptcat {
    namespace clock {
        struct LapInfo {
        private:
            bool is_running_;
            PChrono::microseconds time_;//elapsed time
            PSteadyClock::time_point time_points_;
        public:
            LapInfo() : is_running_{true}, time_{PChrono::microseconds(0)}, time_points_(PSteadyClock::now()){}
            LapInfo(bool is_running, PChrono::microseconds time) : is_running_{is_running}, time_{time}, time_points_(PSteadyClock::now()){}
            LapInfo(bool is_running, PChrono::microseconds time, PSteadyClock::time_point time_points) : is_running_{is_running}, time_{time}, time_points_(time_points){}

            const PSteadyClock::time_point& GetTimePoints() {
                return this->time_points_;
            }

            void Reset() {
                is_running_ = true;
                time_points_ = PSteadyClock::now();
            }

            bool IsRunning(){return is_running_;}

            void AddTime(PChrono::microseconds new_time) {
                time_ += new_time;
            }

            const PChrono::microseconds& GetTime() {
                return time_;
            }

            void SetIsRunning(bool is_running) {
                is_running_ = is_running;
            }

        };

        typedef std::map<std::string, LapInfo> LapCollection;



        /// <summary>
        /// 用于计时当前程序大致使用了多少秒
        /// </summary>
        class DLL_API StopWatch final : public ChronoGraph{
        private:
            LapCollection laps_;//lap times
        public:
            //这里推荐初始化列表使用花括号，更现代，因为对于容器类型数据会更友好， vector{1, 2, 3}
            StopWatch();

            ~StopWatch();

            StopWatch(const StopWatch& cg) = delete;
            StopWatch(const StopWatch&& cg) = delete;
            StopWatch operator=(const StopWatch&) = delete;
            StopWatch& operator=(const StopWatch&&) = delete;

            void Clear(std::string name = "");

            void LapStart(const std::string& name);//start time

            void LapStop(const std::string& name);//stop time

            PLLONG MicroLapTime(const std::string& name);

            PLLONG MilliLapTime(const std::string& name);

            PLLONG LapTime(const std::string& name);
        };
    }

}


#endif //PDATETIME_H
