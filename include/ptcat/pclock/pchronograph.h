//
// Created by 27852 on 2025/5/27.
//

#ifndef CHRONOGRAPH_H
#define CHRONOGRAPH_H

#include "ptcat/common/common.h"
#include <atomic>
#include <shared_mutex>

namespace ptcat {
    namespace clock {

        //
        class DLL_API ChronoGraph {
        protected:
            PChrono::microseconds elapsed_time_;//single timing
            std::atomic_bool is_running_;//current status of running

            PSteadyClock::time_point start_time_;//start time

            //使用 shared mutex, 确保读的时候可以有多个线程访问，写的时候只有一个线程访问，写的时候使用 unique_lock, 读的时候使用 shared_lock
            //Set to mutable type to ensure that it can be modified in const functions
            mutable std::shared_mutex s_mux_;//ensure thread safety,
        public:
            //这里推荐初始化列表使用花括号，更现代，因为对于容器类型数据会更友好， vector{1, 2, 3}
            ChronoGraph();

            virtual ~ChronoGraph();

            ChronoGraph(const ChronoGraph& cg) = delete;
            ChronoGraph(const ChronoGraph&& cg) = delete;
            ChronoGraph operator=(const ChronoGraph&) = delete;
            ChronoGraph& operator=(const ChronoGraph&&) = delete;

            //judgement whether is's currenr runing
            bool IsRunning() const {
                return is_running_.load();
            }

            void Start();//start time

            PLLONG Stop();//stop time and return timing count, in microseconds

            void Reset();// stop and reset timing

            void ReStart();// restart timing

            /// <summary>
            /// get consum time, unit: μs
            /// </summary>
            /// <returns></returns>
            PLLONG MicroElapsedTime() const;
            /// <summary>
            /// get consum time, unit: ms
            /// </summary>
            /// <returns></returns>
            PLLONG MilliElapsedTime() const;

            /// <summary>
            /// get consum time, unit: s
            /// </summary>
            /// <returns></returns>
            PLLONG ElapsedTime() const;
        };
    }
}

#endif //CHRONOGRAPH_H
