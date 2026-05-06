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
            return select::quicksel::QuickSelect(ptr, count, k_index);
        }

        //快速选择
        double QuickSelect(ptcat::Cat1D<double> arr, int k_index){
            double* ptr = arr.Data();
            int count = arr.Cols();
            return select::quicksel::QuickSelect(ptr, count, k_index);
        }
    }
}
