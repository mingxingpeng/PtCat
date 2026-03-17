//
// Created by 27852 on 2026/3/16.
//

#ifndef PTCAT_PSELECT_H
#define PTCAT_PSELECT_H

#include "ptcat/common/common.h"

namespace ptcat {
    namespace palgo {//算法命名空间
        //快速选择
        DLL_API float QuickSelect(float* arr, int count, int k_index);

        //快速选择
        DLL_API double QuickSelect(double* arr, int count, int k_index);

        //中位数获取
        DLL_API float GetMedianElement(float* values, int count);

        //中位数获取
        DLL_API double GetMedianElement(double* values, int count);
    }
}


#endif //PTCAT_PSELECT_H
