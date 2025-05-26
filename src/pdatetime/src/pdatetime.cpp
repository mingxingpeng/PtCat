//
// Created by 27852 on 2025/5/15.
//

#include "pdatetime.h"
#include <cassert>

namespace ptcat {
    namespace datetime {
        StopWatch::StopWatch(): use_time_{ 0 }, start_time_{}, is_runing_{false} {}

        StopWatch::~StopWatch(){}

        //reset prama
        void StopWatch::Reset() {
            is_runing_ = false;
            use_time_ = 0;
            start_time_ = PSteadyClock::time_point();//set default value
        }

        //start record
        void StopWatch::Start(){
            assert(!is_runing_, "currently runing");
            start_time_ = PSteadyClock::now();
            is_runing_ = true;//indicates that it is runing
        }

        //stop record
        void StopWatch::Stop() {
            auto end_time_ = PSteadyClock::now();
            use_time_ = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time_ - start_time_).count();
            Reset();
        }
    }
}
