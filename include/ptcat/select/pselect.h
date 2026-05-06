//
// Created by 27852 on 2026/3/16.
//

#ifndef PTCAT_PSELECT_H
#define PTCAT_PSELECT_H

#include "ptcat/common/common.h"
#include "ptcat/common/cat.hpp"

namespace ptcat {
    namespace palgo {//算法命名空间
        //快速选择
        DLL_API float QuickSelect(ptcat::Cat1D<float> arr, int k_index);

        //快速选择
        DLL_API double QuickSelect(ptcat::Cat1D<double> arr, int k_index);
    }
}


#endif //PTCAT_PSELECT_H
