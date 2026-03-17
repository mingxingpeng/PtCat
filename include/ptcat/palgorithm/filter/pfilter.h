//
// Created by 27852 on 2026/3/12.
//

#ifndef PTCAT_PFILTER_H
#define PTCAT_PFILTER_H

#include "ptcat/common/common.h"
#include <memory>

namespace ptcat{
    namespace palgo{//算法命名空间
        //中值滤波
        DLL_API void MediaFilter(double* data, int height, int width, int kernel);

        //中值滤波
        DLL_API void MediaFilter(float* data, int height, int width, int kernel);

        //高斯滤波
        DLL_API void GaussianFilter(double* data, int height, int width, int kernel);

        //高斯滤波
        DLL_API void GaussianFilter(float* data, int height, int width, int kernel);

        //高斯滤波
        DLL_API void GaussianFilter(double* data, double* out, int height, int width, int kernel);

        //高斯滤波
        DLL_API void GaussianFilter(float* data, float* out, int height, int width, int kernel);

        DLL_API std::pair<int, std::shared_ptr<const double[]>> GetLogKernel(double sigma, int kernel_size = 0);

        DLL_API std::pair<int, std::shared_ptr<const float[]>> GetLogKernel(float sigma, int kernel_size = 0);

        //高斯拉普拉斯滤波
        DLL_API void LaplacianOfGaussianFilter(double* data, double* out, int height, int width, double sigma, int kernel = 0);

        //高斯拉普拉斯滤波
        DLL_API void LaplacianOfGaussianFilter(float* data, float* out, int height, int width, float sigma, int kernel = 0);

        //高斯拉普拉斯滤波
        DLL_API void LaplacianOfGaussianFilter(unsigned char* data, double* out, int height, int width, double sigma, int kernel = 0);

        //高斯拉普拉斯滤波
        DLL_API void LaplacianOfGaussianFilter(unsigned char* data, float* out, int height, int width, float sigma, int kernel = 0);

    }
}
#endif //PTCAT_PFILTER_H
