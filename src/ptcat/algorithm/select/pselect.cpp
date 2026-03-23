//
// Created by 27852 on 2026/3/16.
//

#include "ptcat/select/pselect.h"
#include "select/pquicksel.hpp"

namespace ptcat {
    namespace palgo {//算法命名空间
        //快速选择
        float QuickSelect(ptcat::Cat1D<float> arr, int k_index){
            float* ptr = arr.Data();
            int count = arr.Cols();
            return quicksel::QuickSelect(ptr, count, k_index);
        }

        //快速选择
        double QuickSelect(ptcat::Cat1D<double> arr, int k_index){
            double* ptr = arr.Data();
            int count = arr.Cols();
            return quicksel::QuickSelect(ptr, count, k_index);
        }

        //中位数获取
        float GetMedianElement(ptcat::Cat1D<float> values){
            float* ptr = values.Data();
            int count = values.Cols();
            return quicksel::GetMedianElement(ptr, count);
        }

        //中位数获取
        double GetMedianElement(ptcat::Cat1D<double> values){
            double* ptr = values.Data();
            int count = values.Cols();
            return quicksel::GetMedianElement(ptr, count);
        }

    }
}
