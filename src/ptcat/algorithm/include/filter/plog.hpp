//
// Created by 27852 on 2026/3/16.
//

#ifndef PTCAT_PLOG_HPP
#define PTCAT_PLOG_HPP

#include "../pmemorypool.hpp"
#ifdef _OPENMP
#include <omp.h>
#endif
#include <unordered_map>
#include <shared_mutex>
#include "pmath.hpp"
#include "ptcat/common/cat.hpp"

namespace ptcat {
    namespace palgo {//算法命名空间
        namespace log{
            /*
                LoG 算子是对高斯函数直接求拉普拉斯，而不是先滤波再求导（数学上二者等价，但直接求导可得到解析表达式，便于离散化）
                在图像处理和数字信号处理领域，log 核（Laplacian of Gaussian 核）是二维核,我们这里代码使用的也是二维核
                生成 LoG 核只需要 2 个核心参数
                    这两个参数缺一不可，其他都能自动推导出来。
                    1. 核尺寸 kernel_size
                    必须是 奇数：3、5、7、9、11...
                    作用：决定卷积核多大（3×3、5×5、7×7...）
                    越大，检测的边缘越粗、抗噪越好，但速度越慢
                    2. 高斯标准差 sigma (σ)
                    浮点数，一般用 0.8 ~ 3.0
                    作用：控制模糊程度 + 边缘检测灵敏度
                    小 σ（≈1.0）：边缘细、抗噪差
                    大 σ（≈2.0）：边缘粗、抗噪好
                默认 kernel_size = 6 sigma
                    为什么是 6*sigma
                        高斯分布（正态分布）的特点：
                            ±3σ 范围 包含了 99.7% 的高斯能量
                            超过 3σ 的权重几乎接近 0，对卷积结果几乎没有影响
             */

            struct  LOGKey{
                int kernel_size;//log 核大小
                double sigma;//标准差

                LOGKey() = default;
                LOGKey(int k_size, double sga) : kernel_size(k_size), sigma(sga){}

                bool operator==(const LOGKey& log_key) const{
                    if (this == &log_key)
                        return true;
                    //double 类型相减小于某个值就判为合理
                    return this->kernel_size == log_key.kernel_size && std::abs(this->sigma - log_key.sigma) < 1e-6;
                }
            };

            /*
             LOGKey 要作为 unordered_map 得键，除了 operator== 来分辨，还需要书写哈希函数传给 unordered_map，将 LOGKey 转为一个数，否则会编译报错
             */
            struct LOGKeyHash
            {
                size_t operator()(const LOGKey& k) const {
                    int64_t quantized_sigma = static_cast<int64_t>(k.sigma * 1e6 + 0.5);  // 保留6位小数， 对 sigma 进行量化，使其对微小变化不敏感
                    size_t h1 = std::hash<int>()(k.kernel_size);
                    size_t h2 = std::hash<int64_t>()(quantized_sigma);
                    //组合哈希值
                    return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
                }
            };


            //log 核类
            class LOGKernel final{
            private:
                std::unordered_map<LOGKey, Cat<double>, LOGKeyHash> log_kernels_;//用于存储 log 核
                std::shared_mutex mux_;//共享锁

                //计算 log 核
                Cat<double> CalcLogKernel(const LOGKey& log_key){
                    const double& sigma = log_key.sigma;
                    const int& kernel_size = log_key.kernel_size;
                    //获取到核的一半
                    int radius = kernel_size / 2;
                    //下面的参数是对高斯函数求二阶导后的参数，直接算的，高斯函数对 x 与 y 方向的二阶导我计算过了
                    double sigma2 = sigma * sigma;
                    double sigma4 = sigma2 * sigma2;
                    double sum = 0.0;
                    int kernel_len = kernel_size * kernel_size;
                    //核结果保存
                    Cat<double> kernel(kernel_size, kernel_size);

                    double pi_sigma4_inv = 1.0 / (P_PI * sigma4);
                    double sigma2_inv = 1.0 / (2 * sigma2);

                    //直接对指针进行操作
                    double* kernel_ptr = kernel.Data();

                    for (int i = 0; i < kernel_size; ++i) {
                        for (int j = 0; j < kernel_size; ++j) {
                            double x = i - radius;
                            double y = j - radius;
                            double r2 = x * x + y * y;
                            double value = -1.0 * pi_sigma4_inv * (1.0 - r2 * sigma2_inv) * exp(-r2 * sigma2_inv);
                            int index = i * kernel_size + j;//计算一下索引值
                            kernel_ptr[index] = value;
                            sum += value;
                        }
                    }
                    // 归一化使总和为0
                    if (std::abs(sum) > EPSILON) {
                        double mean = sum / kernel_len;
                        for (int i = 0; i < kernel_len; ++i)
                            kernel_ptr[i] -= mean;
                    }
                    //将获取到的核给存储下来
                    return kernel;
                }

            public:
                LOGKernel() = default;
                ~LOGKernel() = default;

                LOGKernel(const LOGKernel& p) = delete;
                LOGKernel(const LOGKernel&& p) = delete;
                LOGKernel& operator=(const LOGKernel& p) = delete;
                LOGKernel& operator=(const LOGKernel&& p) = delete;

                static LOGKernel& Instance(){
                    static LOGKernel log_kernel;
                    return log_kernel;
                }

                void CoorectKernelSize(double sigma, int& kernel_size){
                    //如果核大小不为奇数，转为奇数
                    if (kernel_size == 0)
                        kernel_size = static_cast<int>(std::ceil(6 * sigma));
                    if (kernel_size % 2 == 0)
                        kernel_size += 1;
                }

                Cat<double> Get(double sigma, int kernel_size){
                    CoorectKernelSize(sigma, kernel_size);

                    LOGKey log_key(kernel_size, sigma);
                    //读操作
                    {
                        std::shared_lock<std::shared_mutex> lock(mux_);
                        //查询当前 key 值有没有对应的核
                        auto it = log_kernels_.find(log_key);
                        if (it != log_kernels_.end()){
                            return it->second;
                        }
                    }
                    //写操作
                    {
                        std::unique_lock<std::shared_mutex> lock(mux_);
//                        std::cout << "写入核为 " << kernel_size << " 的 log 核" << std::endl;
                        auto it = log_kernels_.find(log_key);
                        if (it != log_kernels_.end()){
                            return it->second;
                        }else{
                            auto kernel = CalcLogKernel(log_key);
                            log_kernels_.emplace(log_key, kernel);
                            return kernel;
                        }

                    }
                }
            };


            inline LOGKernel& GetLOG(){
                return LOGKernel::Instance();
            };

            //对于 log 核，只允许 double 和 float 类型
            //对外开放 log 核：
            inline Cat<double> GetLogKernel(double sigma, int kernel_size = 0){
                return GetLOG().Get(sigma, kernel_size);
            }

            //对外开放 log 核：
            inline Cat<float> GetLogKernel(float sigma, int kernel_size = 0){
                auto& log_instance = GetLOG();
                auto kernel = log_instance.Get(sigma, kernel_size);
                int cols = kernel.Cols();
                int rows = kernel.Rows();
                int kernel_len = cols * rows;
                Cat<float> f_kernel(rows, cols);
                double* kernel_ptr = kernel.Data();
                float* f_kernel_ptr = f_kernel.Data();
                for (int i = 0; i < kernel_len; ++i) {
                    f_kernel_ptr[i] = static_cast<float>(kernel_ptr[i]);
                }
                return f_kernel;
            }


            //高斯拉普拉斯滤波
            template<class IN, class OUT>
            inline
            typename std::enable_if<(std::is_same_v<IN, float> || std::is_same_v<IN, double> || std::is_same_v<IN, unsigned char>) &&
                            (std::is_same_v<OUT, float> || std::is_same_v<OUT, double>)>::type
            LaplacianOfGaussianFilter(const IN* data, OUT* out, int height, int width, double sigma, int kernel_size = 0){
                if (!data || !out || height <= 0 || width <= 0 || kernel_size < 0) throw std::invalid_argument("invalid parameter");

                auto& log_instance = GetLOG();
                //查看一下当前核
                auto kernel = log_instance.Get(sigma, kernel_size);
                //获取到核指针
                auto kernel_data = kernel.Data();
                kernel_size = kernel.Rows();//这里是可以确认核宽高是一致的

                int radius = kernel_size / 2;//获取到核半径

                //提前循环修改类型，防止在下面循环里修改
                auto& mp = GetMP();
                int kernel_len = kernel_size * kernel_size;
                OUT* kernel_ptr = mp.template Allocate<OUT>(kernel_len);
                for (int i = 0; i < kernel_len; ++i) {
                    kernel_ptr[i] = static_cast<OUT>(kernel_data[i]);
                }

                //遍历每一个图像
                #pragma omp parallel for
                for (int h = 0; h < height; h++)
                {
                    int h_count = h * width;
                    for (int w = 0; w < width; w++)
                    {
                        //对每一个数值进行卷积
                        OUT sum = 0.0;
                        //对于非边缘区域，不需要对 ny 和 nx 进行限制
                        if (h >= radius && h < height - radius && w >= radius && w < width - radius){
                            for (int ky = -radius; ky <= radius; ++ky) {
                                const IN* data_ptr = &data[(h + ky) * width + (w - radius)];
                                OUT* k_ptr = &kernel_ptr[(ky + radius) * kernel_size];
                                for (int kx = 0; kx < kernel_size; ++kx) {
                                    sum += data_ptr[kx] * k_ptr[kx];
                                }
                            }
                        }else{
                            for (int ky = -radius; ky <= radius; ++ky) {
                                int ny = std::max(0, std::min(height - 1, h + ky));
                                int row_count = ny * width;
                                int k_row_count = (ky + radius) * kernel_size;
                                for (int kx = -radius; kx <= radius; ++kx) {
                                    int nx = std::max(0, std::min(width - 1, w + kx));
                                    int index = row_count + nx;
                                    int kernel_index = k_row_count + kx + radius;
                                    sum += data[index] * kernel_ptr[kernel_index];
                                }
                            }
                        }
                        int sub_index = h_count + w;
                        out[sub_index] = std::abs(sum);
                    }
                }
                mp.DeAllocate(kernel_ptr, kernel_len);
            }
        }
    }
}

#endif //PTCAT_PLOG_HPP
