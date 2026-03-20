//
// Created by 27852 on 2026/3/16.
//

#include "ptcat/select/pselect.h"
#include "select/pquicksel.hpp"

namespace ptcat {
    namespace palgo {//算法命名空间
        //快速选择
        float QuickSelect(float* arr, int count, int k_index){
            return quicksel::QuickSelect(arr, count, k_index);
        }

        //快速选择
        double QuickSelect(double* arr, int count, int k_index){
            return quicksel::QuickSelect(arr, count, k_index);
        }

        //中位数获取
        float GetMedianElement(float* values, int count){
            return quicksel::GetMedianElement(values, count);
        }

        //中位数获取
        double GetMedianElement(double* values, int count){
            return quicksel::GetMedianElement(values, count);
        }

    }
}
