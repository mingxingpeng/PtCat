//
// Created by 27852 on 2026/3/13.
//


        //
// Created by 27852 on 2026/3/13.
//

/*
求线性拟合，根据公式 y = kx + b, 根据最小二乘法的定律，当样本值减去实际值的平方和达到最小时，这个时候的样本数据与原始数据最为接近，也就是就 yi - (kxi + b) 的平方和求其极值，数学上求极值通常是对其求导并且令导数等于0.
最小二乘法公式：
    n						n
    ∑	(yi - f(xi))^2  =>  ∑ (yi - (kxi + b))^2
    i=0						i=0
    对其求偏导并让其导数为 0，然后得到 k 与 b：
                  n∑xiyi - ∑xi∑yi			 ∑yi - k∑xi
        得到 k = ----------------------		b = --------------
                  n∑(xi^2) - (∑xi)^2				  n
        或者使用均值表达式：(X =  1/n * ∑xi, Y = 1/n * ∑yi, XY 为平均值)
                  ∑(xi - X)(yi - Y)
        得到 k = ----------------------		b = Y - kX
                      ∑(xi - X)^2

    对于我这里的调用，我的 x 值是固定个数，固定大小的，所以 x 的平均值是可以提前求出来的,甚至 x 的平均累加也可以提前求出来
                  ∑(xi - X)(yi - Y)			 (xi - X)
        得到 k = ----------------------	==》 ∑-------------- * (yi - Y)
                      ∑(xi - X)^2				∑(xi - X)^2
                              (xi - X)                    (xi - X)
            可以提前求出 ∑-------------- * (yi - Y) 中 --------------
                             ∑(xi - X)^2				 ∑(xi - X)^2

*/

#include "ptcat/fft/pfft.h"
#include "../include/pmemorypool.hpp"

namespace ptcat {
    namespace palgo {//算法命名空间
        namespace fft{
            const static double TwoPi = 2 * P_PI;
            const static double Coefficient = -(10000 / TwoPi);//通过斜率乘以该系数计算高度

            static PMemoryPool& MPool = GetMP();

            PFFT::PFFT() : is_full_spectrum_(false),
                           variable_range_({0, 0}),
                           variable_range_size_(0),
                           init_range_({0, 0}),
                           init_range_size_(0),
                           ft_tri_(nullptr),
                           ft_size_(0),
                           least_squares_sum_x_(nullptr),
                           least_squares_size_(0),
                           N_(0)
            {
            }

            PFFT::~PFFT(){
                PFFTDeInit();
            }

            //初始化所需资源
            void PFFT::PFFTInit(const int& N, const Range& range, bool is_full_spectrum) {
                //为类变量赋值
                if (N <= 0) throw std::invalid_argument("invalid argument");
                bool is_same = (N_ == N) && (init_range_ == range) && (is_full_spectrum_ == is_full_spectrum);
                N_ = N;
                variable_range_ = range;
                init_range_ = range;

                //查看是否使用全谱
                is_full_spectrum_ = is_full_spectrum;
                if (is_full_spectrum_){//如果使用全谱就设置下 variable_range_ 的值
                    variable_range_.start = 0;
                    variable_range_.end = N_ - 1;
                }

                //计算指定区域得傅里叶变换复数，或者全局傅里叶变换复数，也就是说是否计算全谱傅里叶变换得复数
                variable_range_size_ = variable_range_.end - variable_range_.start + 1;
                init_range_size_ = init_range_.end - init_range_.start + 1;
                int N2 = N_ * 2;
                int ft_size = variable_range_size_ * N2;//使用 double 类型，两个 double 表示一个复数
                //首先先判断一下是否需要修改数据获取内存
                if (!ft_tri_ || ft_size_ != ft_size || !is_same) {//判断下数据是否一致
                    //调用内存池对数据进行获取
                    if (ft_tri_)
                        MPool.DeAllocate(ft_tri_, ft_size_);//删除上一个数据得数据长度
                    ft_size_ = ft_size;//然后才更新当前数据的长度，并申请内存
                    ft_tri_ = MPool.Allocate<double>(ft_size_);//申请内存

                    double N_dif = 1.0 / N_;
                    //进行并行运算
#pragma omp parallel for
                    for (int i = 0; i < variable_range_size_; i++)
                    {
                        int k = variable_range_.start + i;
                        //计算一下数据的位置
                        double* curr_ft_tri = &(ft_tri_[i * N2]);
                        for (int n = 0, index = 0; n < N_; n++, index += 2)
                        {
                            double param = TwoPi * k * N_dif * n;
                            double cos_res = std::cos(param);
                            double sin_res = -std::sin(param);
                            curr_ft_tri[index] = cos_res;
                            curr_ft_tri[index + 1] = sin_res;
                        }
                    }

                    //least_squares_sum_x_
                    if (!least_squares_sum_x_ || variable_range_size_ != least_squares_size_){
                        if (least_squares_sum_x_)
                            MPool.DeAllocate(least_squares_sum_x_, least_squares_size_);
                        least_squares_size_ = variable_range_size_;
                        least_squares_sum_x_ = MPool.Allocate<double>(least_squares_size_);
                    }
                    //得到平均值
                    double X = 0;
                    for (int xi = 0; xi < least_squares_size_; xi++)
                    {
                        X += xi;
                    }
                    X /= least_squares_size_;
                    //计算 ∑(xi - X)^2
                    double sum_residual = 0;
                    for (int xi = 0; xi < least_squares_size_; xi++)
                    {
                        sum_residual += std::pow(xi - X, 2);
                    }
                    /*
                               (xi - X)
                    计算	--------------
                             ∑(xi - X)^2
                    */
                    double dif_sum_residual = 1.0 / sum_residual;
                    for (int xi = 0; xi < least_squares_size_; xi++)
                    {
                        least_squares_sum_x_[xi] = (xi - X) * dif_sum_residual;
                    }
                }
            }

            //释放资源
            void PFFT::PFFTDeInit() {
                if (ft_tri_)
                {
                    MPool.DeAllocate(ft_tri_, ft_size_);
                    ft_tri_ = nullptr;
                }
                if (least_squares_sum_x_){
                    MPool.DeAllocate(least_squares_sum_x_, least_squares_size_);
                    least_squares_sum_x_ = nullptr;
                }
            }

            void PFFT::GenerateFrequency(double*& frequency, int& fre_len){
                fre_len = variable_range_size_ * 2;
                frequency = MPool.Allocate<double>(fre_len);
            }

            //创建傅里叶变换计划
            void PFFT::ExecutePFTPlan(const double* input, double*& frequency) {
                //计算出频域数据，只计算范围内的频域数据,其他都为 0，对于接下来的计算没有意义
//#pragma omp parallel for//实行批量计算
                int N2 = N_ * 2;
                for (int i = 0, fre_i = 0; i < variable_range_size_; i++, fre_i += 2)
                {
                    double sum_real = 0;
                    double sum_imag = 0;
                    double* curr_ft_tri = &(ft_tri_[i * N2]);
                    for (int n = 0, index = 0; n < N_; n++, index += 2)
                    {
                        double i_value = input[n];
                        sum_real += curr_ft_tri[index] * i_value;
                        sum_imag += curr_ft_tri[index + 1] * i_value;
                    }
                    frequency[fre_i] = sum_real;
                    frequency[fre_i + 1] = sum_imag;
                }
            }

            // SIMD 加速傅里叶变换
            void PFFT::ExecutePFTPlanAVX(const double* input, double*& frequency) {
                const int step = 4; // 每次处理 4 个 double（256 bit）
                const int step2 = 2 * step; // 每次处理 4 个 double（256 bit）
//#pragma omp parallel for//实行批量计算
                int N2 = N_ * 2;
                for (int i = 0, fre_i = 0; i < variable_range_size_; ++i, fre_i += 2) {
                    __m256d sum_real = _mm256_setzero_pd();
                    __m256d sum_imag = _mm256_setzero_pd();

                    double* curr_ft_tri = &(ft_tri_[i * N2]);
                    int n = 0;
                    int index = 0;
                    for (; n <= N_ - step; n += step, index += step2) {
                        __m256d in_vec = _mm256_loadu_pd(input + n);
                        __m256d tri_real = _mm256_set_pd(
                                curr_ft_tri[index + 6],
                                curr_ft_tri[index + 4],
                                curr_ft_tri[index + 2],
                                curr_ft_tri[index + 0]);
                        __m256d tri_imag = _mm256_set_pd(
                                curr_ft_tri[index + 7],
                                curr_ft_tri[index + 5],
                                curr_ft_tri[index + 3],
                                curr_ft_tri[index + 1]);

                        sum_real = _mm256_fmadd_pd(in_vec, tri_real, sum_real);
                        sum_imag = _mm256_fmadd_pd(in_vec, tri_imag, sum_imag);
                    }
                    // 累加 SIMD 寄存器内容
                    double real_sum[4], imag_sum[4];
                    _mm256_storeu_pd(real_sum, sum_real);
                    _mm256_storeu_pd(imag_sum, sum_imag);

                    double total_real = real_sum[0] + real_sum[1] + real_sum[2] + real_sum[3];
                    double total_imag = imag_sum[0] + imag_sum[1] + imag_sum[2] + imag_sum[3];

                    // 处理剩余项
                    for (; n < N_; ++n, index += 2) {
                        double val = input[n];
                        total_real += curr_ft_tri[index] * val;
                        total_imag += curr_ft_tri[index + 1] * val;
                    }

                    frequency[fre_i] = total_real;
                    frequency[fre_i + 1] = total_imag;
                }
            }

            //创建逆傅里叶变换计划, 做全谱的操作
            void PFFT::ExecutePIFTPlan(double*& out, double*& frequency) {
                //这里计算的是幅度（包络），实部振幅，虚部表示相位，对它俩取模得到结果，获取到幅度信息
                //可以使用 abs 进行取模，也可以使用下面的方式进行取模
                const double N_inv = 1.0 / N_;
                //进行 ifft
                //因为 frequency 其他范围内的值都为 0，没有参与计算的必要
                //ComplexD* result = new ComplexD[N];//数据不需要传出去，这里可以删除，节省 new 和 delete 时间
//#pragma omp parallel for//实行批量计算
                int N2 = 2 * N_;
                for (int n = 0, index = 0; n < N_; n++, index += 2)
                {
                    double sum_real = 0;
                    double sum_imag = 0;
                    for (int i = 0, fre_i = 0; i < variable_range_size_; i++, fre_i += 2)
                    {
                        //这里 ift 操作时虚部需要乘以 -1， 要取负，因为ift 计算出来是 + 虚部， ft_tri_ 是 - 虚部
                        //复数相乘,将虚部和实部先使用变量获取，避免频繁调用函数消磨时间
                        int ft_index = i * N2 + index;
                        const double& ft_real = ft_tri_[ft_index];
                        const double& ft_imag = ft_tri_[ft_index + 1];
                        const double& fre_real = frequency[fre_i];
                        const double& fre_imag = frequency[fre_i + 1];
                        sum_real += (ft_real * fre_real + ft_imag * fre_imag);
                        sum_imag += (ft_real * fre_imag - ft_imag * fre_real);
                    }
                    out[n] = std::sqrt(sum_real * sum_real + sum_imag * sum_imag) * N_inv;
                }
            }

            void PFFT::FreeFrequency(double*& frequency){
                if (frequency){
                    MPool.DeAllocate(frequency, variable_range_size_ * 2);
                }
            }



            //使用 SMID 加速后，速度比 未使用 SMID 的 FFTRun 快大概 100 ms
            void PFFTN::PFFTRun(const double* input, double*& output) {
                double* frequency = nullptr;
                int fre_len = 0;
                GenerateFrequency(frequency, fre_len);
                ExecutePFTPlanAVX(input, frequency);
                ExecutePIFTPlan(output, frequency);
                FreeFrequency(frequency);
            }

            //相位和振幅通常需要全谱信息
            //幅度可能只需要特定频段的信息, 传输入特定谱段的信息
            void PFFTN::PFFTRun(const double* input, double*& output, double*& amplitudes,  double*& phases) {
                //判断下是不是全谱
                if (!is_full_spectrum_) throw std::runtime_error("please set it to full spectrum during initialization");
                double* frequency = nullptr;
                int fre_len = 0;
                GenerateFrequency(frequency, fre_len);
                ExecutePFTPlanAVX(input, frequency);//获取到全谱频域

                //计算出对应的数据
                const double N_inv = 1.0 / N_;
                int N2 = 2 * N_;
//#pragma omp parallel for
                for (int n = 0, fre_i = 0; n < N_; n++, fre_i += 2)
                {
                    const double& out_imag = frequency[fre_i + 1];
                    const double& out_real = frequency[fre_i];
                    //计算相位
                    // 计算相位（使用atan2处理四象限）
                    double phase = atan2(out_imag, out_real);
                    phases[n] = phase; // [-π, π]
                    //计算振幅
                    double amp = std::sqrt(out_real * out_real + out_imag * out_imag) * N_inv;// 除以 N 进行归一化得到实际值
                    amplitudes[n] = amp;
                    //计算幅度， 幅度使用计算范围 calc_range
                    double sum_real = 0;
                    double sum_imag = 0;
                    for (int i = 0; i < init_range_size_; i++)
                    {
                        int calc_index = init_range_.start + i;
                        int ft_index = calc_index * N2 + fre_i;
                        int fre_index = calc_index * 2;
                        //这里 ift 操作时虚部需要乘以 -1， 要取负，因为ift 计算出来是 + 虚部， ft_tri_ 是 - 虚部
                        //复数相乘,将虚部和实部先使用变量获取，避免频繁调用函数消磨时间
                        const double& ft_real = ft_tri_[ft_index];
                        const double& ft_imag = ft_tri_[ft_index + 1];
                        const double& fre_real = frequency[fre_index];
                        const double& fre_imag = frequency[fre_index + 1];
                        sum_real += (ft_real * fre_real + ft_imag * fre_imag);
                        sum_imag += (ft_real * fre_imag - ft_imag * fre_real);
                    }
                    output[n] = std::sqrt(sum_real * sum_real + sum_imag * sum_imag) * N_inv;
                }
                FreeFrequency(frequency);
            }


            /// <summary>
            /// 根据相位来求得数据高度，相位的斜率与包络图高度是成正比的，看相位获取的高度是否比包络图效果好
            /// 返回斜率
            /// </summary>
            void PFFTN::PFFTPhaseRun(const double* input, double& k, double& b) {
                double* frequency = nullptr;
                int fre_len = 0;
                GenerateFrequency(frequency, fre_len);
                //计算出频域数据，只计算范围内的频域数据,其他都为 0，对于接下来的计算没有意义，所以直接只计算范围内的频域数据，免得后续手动置零了
                ExecutePFTPlanAVX(input, frequency);
                //根据这段范围内的频域数据，因为我只取对应范围内的数据， 获取到相位数据，
                double* phases = MPool.Allocate<double>(variable_range_size_);//存储振幅数据
                //振幅计算公式
                int n = 0;
                double last_phase = 0;
                for (int i = 0, fre_i = 0; i < variable_range_size_; i++, fre_i += 2)
                {
                    // 计算相位（使用atan2处理四象限）, -- pmx 这里使用 三角函数，可能会有性能消耗，后续优化使用 SIMD
                    phases[i] = std::atan2(frequency[fre_i + 1], frequency[fre_i]); // [-π, π]
                    //将其绘制成为还原回一条直线
                    if (i != 0)//首位不需要设置
                        if (phases[i] > last_phase) n++;
                    last_phase = phases[i];
                    phases[i] -= n * fft::TwoPi;
                }

                //对数据做最小二乘法
                //计算 Y
                double Y = 0;
                for (int i = 0; i < variable_range_size_; i++)
                {
                    Y += phases[i];
                }
                Y /= variable_range_size_;
                //计算出斜率
                k = 0;
                for (int i = 0; i < variable_range_size_; i++)
                {
                    k += least_squares_sum_x_[i] * (phases[i] - Y);
                }

                //做一个计算 b 的处理，实际代码中，这里会被注释掉
                double X = 0;
                for (int xi = 0; xi < variable_range_size_; xi++)
                {
                    X += xi;
                }
                X /= variable_range_size_;
                b = Y - k * X;

                //释放资源
                if (phases)
                    MPool.DeAllocate(phases, variable_range_size_);
                FreeFrequency(frequency);
            }
        }
    }
}
