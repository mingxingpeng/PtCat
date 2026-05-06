//
// Created by 27852 on 2026/5/6.
//

#ifndef PTCAT_PSTATISTICS_H
#define PTCAT_PSTATISTICS_H
#include "ptcat/common/common.h"
#include "ptcat/common/cat.hpp"

namespace ptcat {
    namespace palgo {//算法命名空间
        //中位数获取
        DLL_API float GetMedianElement(ptcat::Cat1D<float> values);

        //中位数获取
        DLL_API double GetMedianElement(ptcat::Cat1D<double> values);

        //加权平均获取峰值索引
        DLL_API void GetPeakIndexByWAvg(ptcat::Cat1D<float> input, int weight, float& peak_index);

        //加权平均获取峰值索引
        DLL_API void GetPeakIndexByWAvg(ptcat::Cat1D<double> input, int weight, double& peak_index);
    }
}


#endif //PTCAT_PSTATISTICS_H
