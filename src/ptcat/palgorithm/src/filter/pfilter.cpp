//
// Created by 27852 on 2026/3/13.
//

#include "ptcat/palgorithm/filter/pfilter.h"
#include "filter/filter_core.h"



namespace ptcat{
    namespace palgo{//算法命名空间

        //中值滤波
        void MediaFilter(double* data, int height, int width, int kernel){
            core::MediaFilter(data, height, width, kernel);
        }


        //中值滤波， 数据为空就直接报错
        void MediaFilter(float* data, int height, int width, int kernel){
            core::MediaFilter(data, height, width, kernel);
        }
    }
}
