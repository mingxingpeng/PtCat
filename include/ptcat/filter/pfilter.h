//
// Created by 27852 on 2026/3/12.
//

#ifndef PTCAT_PFILTER_H
#define PTCAT_PFILTER_H

#include "ptcat/common/common.h"
#include <memory>
#include "ptcat/common/cat.hpp"

namespace ptcat{
    namespace palgo{//算法命名空间
        //中值滤波
        DLL_API void MediaFilter(Cat<double>& data, int kernel);

        //中值滤波
        DLL_API void MediaFilter(Cat<float>& data, int kernel);

        //高斯滤波
        DLL_API void GaussianFilter(double* data, int height, int width, int kernel);

        //高斯滤波
        DLL_API void GaussianFilter(float* data, int height, int width, int kernel);

        //高斯滤波
        DLL_API void GaussianFilter(double* data, double* out, int height, int width, int kernel);

        //高斯滤波
        DLL_API void GaussianFilter(float* data, float* out, int height, int width, int kernel);

        DLL_API Cat<double> GetLogKernel(double sigma, int kernel_size = 0);

        DLL_API Cat<float> GetLogKernel(float sigma, int kernel_size = 0);

        //高斯拉普拉斯滤波
        DLL_API void LaplacianOfGaussianFilter(const Cat<double>& data, Cat<double>& out, double sigma, int kernel = 0);

        //高斯拉普拉斯滤波
        DLL_API void LaplacianOfGaussianFilter(const Cat<float>& data, Cat<float>& out, float sigma, int kernel = 0);

        //高斯拉普拉斯滤波
        DLL_API void LaplacianOfGaussianFilter(const Cat<uchar>& data, Cat<double>& out, double sigma, int kernel = 0);

        //高斯拉普拉斯滤波
        DLL_API void LaplacianOfGaussianFilter(const Cat<uchar>& data, Cat<float>& out, float sigma, int kernel = 0);

//        void Filter2D(double* data, double* out, );

    }
}
#endif //PTCAT_PFILTER_H
