//
// Created by 27852 on 2025/5/27.
//

#include "chronograph.h"
#include <cassert>

namespace ptcat {
    namespace clock {
        //这里推荐初始化列表使用花括号，更现代，因为对于容器类型数据会更友好， vector{1, 2, 3}
        ChronoGraph::ChronoGraph() : elapsed_time_{ 0 }, start_time_{}, is_running_{false} {}

        ChronoGraph::~ChronoGraph(){}

        //start record
        void ChronoGraph::Start(){
            assert(!is_running_, "currently running...");
            elapsed_time_ = 0;
            is_running_ = true;//indicates that it is runing
            start_time_ = PSteadyClock::now();
        }

        //stop record
        void ChronoGraph::Stop() {
            auto end_time_ = PSteadyClock::now();
            is_running_ = false;
            elapsed_time_ = std::chrono::duration_cast<std::chrono::microseconds>(end_time_ - start_time_).count();
        }
    }
}