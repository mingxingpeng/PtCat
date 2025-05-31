//
// Created by 27852 on 2025/5/15.
//

#include "pclock/pstopwatch.h"
#include <cassert>

namespace ptcat {
    namespace clock {
        //这里推荐初始化列表使用花括号，更现代，因为对于容器类型数据会更友好， vector{1, 2, 3}
        StopWatch::StopWatch(): laps_{} {}

        StopWatch::~StopWatch(){}

        void StopWatch::LapStart(std::string name) {
            Start();
            assert(name != "", "the name cannot be empty...");
            laps_.emplace_back(name, 0);//会自动的调用对应对象的构造函数
        }

        void StopWatch::LapStop() {
            Stop();
            if (laps_.size() > 0) {
                laps_.back().time_ = elapsed_time_;
            }
        }

        PLLONG StopWatch::CalcElapsed(std::string name) {
            PLLONG elapsed = 0;
            for (auto element: laps_) {
                if (name == "") {
                    elapsed += element.time_;
                }else {
                    if (element.name_ == name) {
                        elapsed += element.time_;
                    }
                }
            }
            return elapsed;
        }

        PLLONG StopWatch::MicroLapTime(std::string name) {
            return CalcElapsed(name);
        }

        PLLONG StopWatch::MilliLapTime(std::string name){
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::microseconds(CalcElapsed(name))).count();
        }

        PLLONG StopWatch::LapTime(std::string name){
            return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::microseconds(CalcElapsed(name))).count();
        }

        PLLONG StopWatch::MicroTotalTime(){
            return CalcElapsed();
        }

        PLLONG StopWatch::MilliTotalTime(){
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::microseconds(CalcElapsed())).count();
        }

        PLLONG StopWatch::TotalTime(){
            return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::microseconds(CalcElapsed())).count();
        }

    }
}
