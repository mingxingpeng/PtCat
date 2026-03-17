//
// Created by 27852 on 2026/3/13.
//

#include "ptcat/palgorithm/filter/pfilter.h"
#include "filter/pmedian.h"
#include "filter/plog.h"

namespace ptcat{
    namespace palgo{//算法命名空间
        //中值滤波
        void MediaFilter(double* data, int height, int width, int kernel){
            median::MediaFilter(data, height, width, kernel);
        }

        //中值滤波
        void MediaFilter(float* data, int height, int width, int kernel){
            median::MediaFilter(data, height, width, kernel);
        }

        void GaussianFilter(double* data, int height, int width, int kernel){

        }

        void GaussianFilter(float* data, int height, int width, int kernel){

        }

        void GaussianFilter(double* data, double* out, int height, int width, int kernel){

        }

        void GaussianFilter(float* data, double* out, int height, int width, int kernel){

        }

        std::pair<int, std::shared_ptr<const double[]>> GetLogKernel(double sigma, int kernel_size){
            return log::GetLogKernel(sigma, kernel_size);
        }

        std::pair<int, std::shared_ptr<const float[]>> GetLogKernel(float sigma, int kernel_size){
            return log::GetLogKernel(sigma, kernel_size);
        }

        void LaplacianOfGaussianFilter(double* data, double* out, int height, int width, double sigma, int kernel){
            log::LaplacianOfGaussianFilter(data, out, height, width, sigma, kernel);
        }

        void LaplacianOfGaussianFilter(float* data, float* out, int height, int width, float sigma, int kernel){
            log::LaplacianOfGaussianFilter(data, out, height, width, sigma, kernel);
        }

        //高斯拉普拉斯滤波
        void LaplacianOfGaussianFilter(unsigned char* data, double* out, int height, int width, double sigma, int kernel){
            log::LaplacianOfGaussianFilter(data, out, height, width, sigma, kernel);
        }

        //高斯拉普拉斯滤波
        void LaplacianOfGaussianFilter(unsigned char* data, float* out, int height, int width, float sigma, int kernel){
            log::LaplacianOfGaussianFilter(data, out, height, width, sigma, kernel);
        }
    }
}
