//
// Created by 27852 on 2025/5/27.
//

#include "ptcat/pclock/pchronograph.h"
#include <cassert>

namespace ptcat {
    namespace clock {
        //这里推荐初始化列表使用花括号，更现代，因为对于容器类型数据会更友好， vector{1, 2, 3}
        ChronoGraph::ChronoGraph() : elapsed_time_{0}, start_time_{}, is_running_{false} {}

        ChronoGraph::~ChronoGraph() = default;

        //start timing
        void ChronoGraph::Start(){
            std::unique_lock<std::shared_mutex> lock(s_mux_);
            /*
             assert(!is_running_ && "currently running...");
                正常我会这样写 assert(!is_running_, "currently running...");
                    这样 !is_running_ 的时候会报错 !is_running_，而不会带出后面的信息
                    而像这样写 !is_running_ && "currently running..."
                        会展示 !is_running_ && "currently running..."
             **/
            if (is_running_) throw std::runtime_error("ChronoGraph is already running...");
            // assert(!is_running_ && "ChronoGraph is already running...");
            elapsed_time_ = PChrono::microseconds(0);
            is_running_.store(true);//indicates that it is runing
            start_time_ = PSteadyClock::now();
        }

        //stop record
        PLLONG ChronoGraph::Stop() {
            std::unique_lock<std::shared_mutex> lock(s_mux_);
            //判断程序有没有开启监测
            if (!is_running_) throw std::runtime_error("timing not started...");
            // assert(is_running_ && "timing not started");
            auto end_time_ = PSteadyClock::now();
            is_running_.store(false);
            elapsed_time_ = PChrono::duration_cast<PChrono::microseconds>(end_time_ - start_time_);
            return elapsed_time_.count();
        }

        //stop and reset timing
        void ChronoGraph::Reset() {
            std::unique_lock<std::shared_mutex> lock(s_mux_);
            is_running_.store(false);
            elapsed_time_ = PChrono::microseconds(0);
        }

        //restart timing
        void ChronoGraph::ReStart() {
            std::unique_lock<std::shared_mutex> lock(s_mux_);
            elapsed_time_ = PChrono::microseconds(0);
            is_running_.store(true);//indicates that it is runing
            start_time_ = PSteadyClock::now();
        }

        PLLONG ChronoGraph::MicroElapsedTime() const{
            std::shared_lock<std::shared_mutex> lock(s_mux_);
            return elapsed_time_.count();
        }

        PLLONG ChronoGraph::MilliElapsedTime() const{
            std::shared_lock<std::shared_mutex> lock(s_mux_);
            return PChrono::duration_cast<PChrono::milliseconds>(elapsed_time_).count();
        }

        PLLONG ChronoGraph::ElapsedTime() const{
            std::shared_lock<std::shared_mutex> lock(s_mux_);
            return PChrono::duration_cast<PChrono::seconds>(elapsed_time_).count();;
        }
    }
}