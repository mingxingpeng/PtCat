//
// Created by 27852 on 2025/5/27.
//

#ifndef CHRONOGRAPH_H
#define CHRONOGRAPH_H

#include "common.h"

namespace ptcat {
    namespace clock {

        //
        class ChronoGraph {
        protected:
            PLLONG elapsed_time_;//single timing
            bool is_running_;//current status of running

            PSteadyClock::time_point start_time_;//start time

        public:
            //这里推荐初始化列表使用花括号，更现代，因为对于容器类型数据会更友好， vector{1, 2, 3}
            ChronoGraph();

            virtual ~ChronoGraph();

            //judgement whether is's currenr runing
            bool Running() {
                return is_running_;
            }

            void Start();//start time

            void Stop();//stop time

            /// <summary>
            /// get consum time, unit: μs
            /// </summary>
            /// <returns></returns>
            PLLONG MicroElapsedTime()  const {
                return elapsed_time_;
            }

            /// <summary>
            /// get consum time, unit: ms
            /// </summary>
            /// <returns></returns>
            PLLONG MilliElapsedTime()  const {
                return std::chrono::duration<PLLONG, std::chrono::milliseconds>(elapsed_time_).count();
            }

            /// <summary>
            /// get consum time, unit: s
            /// </summary>
            /// <returns></returns>
            PLLONG ElapsedTime()  const {
                return std::chrono::duration<PLLONG, std::chrono::seconds>(elapsed_time_).count();
            }
        };
    }
}

#endif //CHRONOGRAPH_H
