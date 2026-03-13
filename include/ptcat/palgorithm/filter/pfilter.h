//
// Created by 27852 on 2026/3/12.
//

#ifndef PTCAT_PFILTER_H
#define PTCAT_PFILTER_H

#include "ptcat/common/common.h"


namespace ptcat{
    namespace palgo{//算法命名空间

        //中值滤波
        DLL_API void MediaFilter(double* data, int height, int width, int kernel);
        //中值滤波
        DLL_API void MediaFilter(float* data, int height, int width, int kernel);
    }
}
#endif //PTCAT_PFILTER_H
