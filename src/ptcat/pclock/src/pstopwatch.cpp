//
// Created by 27852 on 2025/5/15.
//

#include "ptcat/pclock/pstopwatch.h"
// #include <cassert> release 模式下会被消除
#include <stdexcept>
#include <sstream>

namespace ptcat {
    namespace clock {
        //这里推荐初始化列表使用花括号，更现代，因为对于容器类型数据会更友好， vector{1, 2, 3}
        StopWatch::StopWatch(): laps_{} {}

        StopWatch::~StopWatch(){}

        void StopWatch::LapStart(const std::string& name) {
            //根据当前名称来判断
            std::unique_lock<std::shared_mutex> lock(s_mux_);
            if (name == "") throw std::runtime_error("the name cannot be empty...");
            // assert(name != "" && "the name cannot be empty...");
            //判断一下是当前名称是否有运行的
            auto it = laps_.find(name);
            if (it == laps_.end()) {// not result
                laps_.emplace(name, LapInfo());
            }else {
                if (it->second.IsRunning()) throw std::runtime_error("The current theme has started timing...");
                // assert(!it->second.IsRunning() && "The current theme has started timing...");
                it->second.Reset();
            }
        }

        void StopWatch::LapStop(const std::string& name) {
            std::unique_lock<std::shared_mutex> lock(s_mux_);
            //判断下当前主题是否存在
            auto it = laps_.find(name);
            if (it == laps_.end()) throw std::runtime_error("No current topic found...");
            // assert(it != laps_.end() && "No current topic found...");
            if(it->second.IsRunning()) {
                //进行计算
                auto end_time_ = PSteadyClock::now();
                it->second.SetIsRunning(false);
                it->second.AddTime(PChrono::duration_cast<PChrono::microseconds>(end_time_ - it->second.GetTimePoints()));
            }
        }

        void StopWatch::Clear(std::string name) {
            std::unique_lock<std::shared_mutex> lock(s_mux_);
            if (name == "") {
                for (auto element: laps_) {
                    std::stringstream ss;
                    ss << "Theme " << element.first << " is still running";
                    if (element.second.IsRunning()) throw std::runtime_error(ss.str());
                    // assert(!element.second.IsRunning(), str);
                }
                laps_.clear();
            }else {
                std::stringstream ss;
                ss << "Theme " << name << " is still running";
                auto it = laps_.find(name);
                if (it == laps_.end()) return;
                if (it->second.IsRunning()) throw std::runtime_error(ss.str());
                // assert(!laps_[name].IsRunning(), str);
                laps_.erase(it);
            }
        }

        PLLONG StopWatch::MicroLapTime(const std::string& name) {
            std::shared_lock<std::shared_mutex> lock(s_mux_);
            auto it = laps_.find(name);
            if (it == laps_.end())
                return 0;
            return it->second.GetTime().count();
        }

        PLLONG StopWatch::MilliLapTime(const std::string& name){
            std::shared_lock<std::shared_mutex> lock(s_mux_);
            auto it = laps_.find(name);
            if (it == laps_.end())
                return 0;
            return std::chrono::duration_cast<std::chrono::milliseconds>(it->second.GetTime()).count();
        }

        PLLONG StopWatch::LapTime(const std::string& name){
            std::shared_lock<std::shared_mutex> lock(s_mux_);
            auto it = laps_.find(name);
            if (it == laps_.end())
                return 0;
            return std::chrono::duration_cast<std::chrono::seconds>(it->second.GetTime()).count();
        }
    }
}
