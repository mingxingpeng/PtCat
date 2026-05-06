//
// Created by 27852 on 2026/3/16.
//

#ifndef PTCAT_FILTER_PGAUSSIAN_H
#define PTCAT_FILTER_PGAUSSIAN_H

#include "../pmemorypool.hpp"
#ifdef _OPENMP
#include <omp.h>
#endif

namespace ptcat {
    namespace palgo {//算法命名空间
        namespace filter{
            namespace gaussian{
                //高斯比较小的核就直接使用预设值

                //高斯滤波
                void GaussianFilter(float* data, float* out, int height, int width, int kernel){

                }

                //高斯滤波
                void GaussianFilter(float* data, int height, int width, int kernel){
                    if (!data || height <= 0 || width <= 0 || kernel <= 0) throw std::invalid_argument("invalid parameter");
                    auto& mpool = GetMP();
                    int data_size = height * width;
                    float* temp = mpool.Allocate<float>(data_size);
                    GaussianFilter(temp, data, height, width, kernel);
                    mpool.DeAllocate(temp, data_size);
                }
            }
        }
    }
}

#endif //PTCAT_FILTER_PGAUSSIAN_H
