//
// Created by 27852 on 2026/3/13.
//

#include "ptcat/filter/pfilter.h"
#include "filter/pmedian.hpp"
#include "filter/plog.hpp"

namespace ptcat{
    namespace palgo{//算法命名空间
        //中值滤波
        void MediaFilter(Cat<double>& data, int kernel){
            if (data.Empty()) throw std::invalid_argument("data is empty");
            //获取到数据
            double* ptr = data.Data();
            int height = data.Rows(),  width = data.Cols();
            median::MediaFilter(ptr, height, width, kernel);
        }

        //中值滤波
        void MediaFilter(Cat<float>& data, int kernel){
            if (data.Empty()) throw std::invalid_argument("data is empty");
            //获取到数据
            float* ptr = data.Data();
            int height = data.Rows(),  width = data.Cols();
            median::MediaFilter(ptr, height, width, kernel);
        }

        void GaussianFilter(double* data, int height, int width, int kernel){

        }

        void GaussianFilter(float* data, int height, int width, int kernel){

        }

        void GaussianFilter(double* data, double* out, int height, int width, int kernel){

        }

        void GaussianFilter(float* data, double* out, int height, int width, int kernel){

        }

        Cat<double> GetLogKernel(double sigma, int kernel_size){
            return log::GetLogKernel(sigma, kernel_size);
        }

        Cat<float> GetLogKernel(float sigma, int kernel_size){
            return log::GetLogKernel(sigma, kernel_size);
        }

        void LaplacianOfGaussianFilter(const Cat<double>& data, Cat<double>& out, double sigma, int kernel){
            if (data.Empty()) throw std::invalid_argument("data is empty");
            int height = data.Rows(), width = data.Cols();
            if (out.Empty())
                out = Cat<double>(height, width);
            else
                //判断一下数据是否有问题
                if (height != out.Rows() || width != out.Cols()) throw std::invalid_argument("inconsistent data length");
            //获取到数据
            const double* data_ptr = data.Data();
            double* out_ptr = out.Data();
            log::LaplacianOfGaussianFilter(data_ptr, out_ptr, height, width, sigma, kernel);
        }

        void LaplacianOfGaussianFilter(const Cat<float>& data, Cat<float>& out, float sigma, int kernel){
            if (data.Empty()) throw std::invalid_argument("data is empty");
            int height = data.Rows(), width = data.Cols();
            if (out.Empty())
                out = Cat<float>(height, width);
            else
                //判断一下数据是否有问题
            if (height != out.Rows() || width != out.Cols()) throw std::invalid_argument("inconsistent data length");
            //获取到数据
            const float* data_ptr = data.Data();
            float* out_ptr = out.Data();
            log::LaplacianOfGaussianFilter(data_ptr, out_ptr, height, width, sigma, kernel);
        }

        //高斯拉普拉斯滤波
        void LaplacianOfGaussianFilter(const Cat<uchar>& data, Cat<double>& out, double sigma, int kernel){
            if (data.Empty()) throw std::invalid_argument("data is empty");
            int height = data.Rows(), width = data.Cols();
            if (out.Empty())
                out = Cat<double>(height, width);
            else
                //判断一下数据是否有问题
            if (height != out.Rows() || width != out.Cols()) throw std::invalid_argument("inconsistent data length");
            //获取到数据
            const uchar* data_ptr = data.Data();
            double* out_ptr = out.Data();
            log::LaplacianOfGaussianFilter(data_ptr, out_ptr, height, width, sigma, kernel);
        }

        //高斯拉普拉斯滤波
        void LaplacianOfGaussianFilter(const Cat<uchar>& data, Cat<float>& out, float sigma, int kernel){
            if (data.Empty()) throw std::invalid_argument("data is empty");
            int height = data.Rows(), width = data.Cols();
            if (out.Empty())
                out = Cat<float>(height, width);
            else
                //判断一下数据是否有问题
            if (height != out.Rows() || width != out.Cols()) throw std::invalid_argument("inconsistent data length");
            //获取到数据
            const uchar* data_ptr = data.Data();
            float* out_ptr = out.Data();
            log::LaplacianOfGaussianFilter(data_ptr, out_ptr, height, width, sigma, kernel);
        }
    }
}
