//
// Created by 27852 on 2026/3/12.
//

#ifndef PTCAT_PFFT_H
#define PTCAT_PFFT_H
#include "ptcat/common/common.h"

#include <atomic>
#include <immintrin.h>
#include <iostream>

#ifdef _OPENMP
#include <omp.h>
#endif
#include "ptcat/pmath.hpp"



#ifndef FT_FORWARD
#define FT_FORWARD -1
#endif

#ifndef FT_BACKWARD
#define FT_BACKWARD +1
#endif

namespace ptcat{
    namespace palgo{//算法命名空间
        namespace fft{

            struct Range{
                int start;
                int end;

                Range(int start, int end) : start(start), end(end){
                    if (start > end) {
                        throw std::invalid_argument("start must be <= end");
                    }
                }

                Range(const Range& range) = default;
                Range& operator=(const Range& range) = default;

                bool operator==(const Range& range){
                    if (this == &range) return true;
                    return this->start == range.start && this->end == range.end;
                }
            };

            class DLL_API PFFT{
            protected:
//                ComplexD* ft_tri_;//用于存储傅里叶变换得复数，反傅里叶变换，虚部乘以 -1
                double* ft_tri_;//用于存储傅里叶变换得复数，反傅里叶变换，虚部乘以 -1, 不再使用结构体来存储，例如 0 是实部， 1 是虚部
                int ft_size_;//数据长度
                Range range_;//传入进来得数据范围, 数据索引从 0 开始
                int range_size_;
                //初始化傅里叶变换复数，是否全谱初始化，还是局部初始化
                bool is_full_spectrum_ = false;//一般来说不推荐全谱，因为我可能要的就是某一个区域得数据，其他区域可以都设置为 0，并没有影响，只有在需要获取振幅，相位等需要用到全谱计算
                double* least_squares_sum_x_;//最小二乘计算
                int least_squares_size_;
                int N_;//数据数量

            public:
                PFFT();

                ~PFFT();

                //初始化所需资源
                void PFFTInit(const int& N, const Range& range, bool is_full_spectrum = false);

                //释放资源
                void PFFTDeInit();
            protected:
                //创建傅里叶变换计划
                void ExecutePFTPlan(const double* input, double*& frequency) ;

                // SIMD 加速傅里叶变换
                void ExecutePFTPlanAVX(const double* input, double*& frequency) ;

                //创建逆傅里叶变换计划, 做全谱的操作
                void ExecutePIFTPlan(double*& out, double*& frequency) ;

                void FreeFrequency(double*& frequency);
            };

            class DLL_API PFFTN final : public fft::PFFT{
            public:
                //使用 SMID 加速后，速度比 未使用 SMID 的 FFTRun 快大概 100 ms
                void PFFTRun(const double* input, double*& output);

                //获取到振幅， 相位，幅度，这里必须全谱
                void PFFTRun(const Range& calc_range, const double* input, double*& output, double*& amplitudes,  double*& phases);


                /// <summary>
                /// 根据相位来求得数据高度，相位的斜率与包络图高度是成正比的，看相位获取的高度是否比包络图效果好
                /// 返回斜率
                /// </summary>
                void PFFTPhaseRun(const double* input, double& k, double& b);
            };
        }
    }
}
#endif //PTCAT_PFFT_H
