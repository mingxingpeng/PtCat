//
// Created by 27852 on 2026/3/24.
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

#include <atomic>
#include <immintrin.h>
#include <iostream>
#include "../pmath.hpp"
#include "../pmemorypool.hpp"
#ifdef _OPENMP
#include <omp.h>
#endif



#ifndef FT_FORWARD
#define FT_FORWARD -1
#endif

#ifndef FT_BACKWARD
#define FT_BACKWARD +1
#endif

#ifndef PTCAT_PFFT_HPP
#define PTCAT_PFFT_HPP


namespace ptcat{
    namespace palgo{
        namespace fft{
            const static double TwoPi = 2 * P_PI;
            const static double Coefficient = -(10000 / TwoPi);//通过斜率乘以该系数计算高度

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

            class PFFT{
            private:
                ComplexD* ft_tri_;//用于存储傅里叶变换得复数，反傅里叶变换，虚部乘以 -1
                int ft_size_;//数据长度
                Range range_;//传入进来得数据范围, 数据索引从 0 开始
                int range_size_;
                //初始化傅里叶变换复数，是否全谱初始化，还是局部初始化
                bool is_full_spectrum_ = false;//一般来说不推荐全谱，因为我可能要的就是某一个区域得数据，其他区域可以都设置为 0，并没有影响，只有在需要获取振幅，相位等需要用到全谱计算
                double* least_squares_sum_x_;//最小二乘计算
                int least_squares_size_;
                int N_;//数据数量
                ComplexD* frequency_;//生成的频域数据
                int frequency_size_;

            public:
                PFFT() : is_full_spectrum_(false),
                range_({0, 0}),
                range_size_(0),
                ft_tri_(nullptr),
                ft_size_(0),
                least_squares_sum_x_(nullptr),
                least_squares_size_(0),
                N_(0),
                frequency_(nullptr),
                frequency_size_(0)
                {
                }

                ~PFFT(){
                    PFFTDeInit();
                }

                //初始化所需资源
                void PFFTInit(const int& N, const Range& range, bool is_full_spectrum = false) {
                    //为类变量赋值
                    if (N <= 0) throw std::invalid_argument("invalid argument");
                    bool is_same = (N_ == N) && (range_ == range);
                    N_ = N;
                    range_ = range;
                    is_full_spectrum_ = is_full_spectrum;
                    if (is_full_spectrum_){//如果使用全谱就设置下 range 得值
                        range_.start = 0;
                        range_.end = N_ - 1;
                    }

                    //计算指定区域得傅里叶变换复数，或者全局傅里叶变换复数，也就是说是否计算全谱傅里叶变换得复数
                    range_size_ = range_.end - range_.start + 1;
                    int ft_size = range_size_ * N;
                    //首先先判断一下是否需要修改数据获取内存
                    if (!ft_tri_ || ft_size_ != ft_size || !is_same) {//判断下数据是否一致
                        //调用内存池对数据进行获取
                        auto &m_pool = GetMP();

                        if (ft_tri_)
                            m_pool.DeAllocate(ft_tri_, ft_size_);//删除上一个数据得数据长度
                        ft_size_ = ft_size;//然后才更新当前数据的长度，并申请内存
                        ft_tri_ = m_pool.Allocate<ComplexD>(ft_size_);//申请内存

                        double N_dif = 1.0 / N;
                        //进行并行运算
#pragma omp parallel for
                        for (int i = 0; i < range_size_; i++)
                        {
                            int k = range_.start + i;
                            //计算一下数据的位置
                            ComplexD* curr_ft_tri = &(ft_tri_[i * N]);
                            for (int n = 0; n < N; n++)
                            {
                                double param = TwoPi * k * N_dif * n;
                                double cos_res = std::cos(param);
                                double sin_res = -std::sin(param);
                                curr_ft_tri[n].real = cos_res;
                                curr_ft_tri[n].imag = sin_res;
                            }
                        }

                        //least_squares_sum_x_
                        if (!least_squares_sum_x_ || range_size_ != least_squares_size_){
                            if (least_squares_sum_x_)
                                m_pool.DeAllocate(least_squares_sum_x_, least_squares_size_);
                            least_squares_size_ = range_size_;
                            least_squares_sum_x_ = m_pool.Allocate<double>(least_squares_size_);
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

                        if (!frequency_ || range_size_ != frequency_size_){
                            if (frequency_)
                                m_pool.DeAllocate(frequency_, frequency_size_);
                            frequency_size_ = range_size_;
                            frequency_ = m_pool.Allocate<ComplexD>(frequency_size_);
                        }
                    }
                }

                //释放资源
                void PFFTDeInit() {
                    auto &m_pool = GetMP();
                    if (ft_tri_)
                    {
                        m_pool.DeAllocate(ft_tri_, ft_size_);
                        ft_tri_ = nullptr;
                    }
                    if (least_squares_sum_x_){
                        m_pool.DeAllocate(least_squares_sum_x_, least_squares_size_);
                        least_squares_sum_x_ = nullptr;
                    }
                    if (frequency_){
                        m_pool.DeAllocate(frequency_, frequency_size_);
                        frequency_ = nullptr;
                    }
                }

                //创建傅里叶变换计划
                void ExecutePFTPlan(const double* input) {
                    //计算出频域数据，只计算范围内的频域数据,其他都为 0，对于接下来的计算没有意义
#pragma omp parallel for//实行批量计算
                    for (int i = 0; i < range_size_; i++)
                    {
                        double sum_real = 0;
                        double sum_imag = 0;
                        ComplexD* curr_ft_tri = &(ft_tri_[i * N_]);
                        for (int n = 0; n < N_; n++)
                        {
                            double i_value = input[n];
                            sum_real += curr_ft_tri[n].real * i_value;
                            sum_imag += curr_ft_tri[n].imag * i_value;
                        }
                        frequency_[i].real = sum_real;
                        frequency_[i].imag = sum_imag;
                    }
                }

                // SIMD 加速傅里叶变换
                void ExecutePFTPlanAVX(const double* input) {
                    const int step = 4; // 每次处理 4 个 double（256 bit）
#pragma omp parallel for//实行批量计算
                    for (int i = 0; i < range_size_; ++i) {
                        __m256d sum_real = _mm256_setzero_pd();
                        __m256d sum_imag = _mm256_setzero_pd();

                        ComplexD* curr_ft_tri = &(ft_tri_[i * N_]);
                        int n = 0;
                        for (; n <= N_ - step; n += step) {
                            __m256d in_vec = _mm256_loadu_pd(input + n);
                            __m256d tri_real = _mm256_set_pd(
                                    curr_ft_tri[n + 3].real,
                                    curr_ft_tri[n + 2].real,
                                    curr_ft_tri[n + 1].real,
                                    curr_ft_tri[n + 0].real);
                            __m256d tri_imag = _mm256_set_pd(
                                    curr_ft_tri[n + 3].imag,
                                    curr_ft_tri[n + 2].imag,
                                    curr_ft_tri[n + 1].imag,
                                    curr_ft_tri[n + 0].imag);

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
                        for (; n < N_; ++n) {
                            double val = input[n];
                            total_real += curr_ft_tri[n].real * val;
                            total_imag += curr_ft_tri[n].imag * val;
                        }

                        frequency_[i].real = total_real;
                        frequency_[i].imag = total_imag;
                    }
                }

                //创建逆傅里叶变换计划, 做全谱的操作
                void ExecutePIFTPlan(double*& out) {
                    //这里计算的是幅度（包络），实部振幅，虚部表示相位，对它俩取模得到结果，获取到幅度信息
                    //可以使用 abs 进行取模，也可以使用下面的方式进行取模
                    const double N_inv = 1.0 / N_;
                    //进行 ifft
                    //因为 frequency 其他范围内的值都为 0，没有参与计算的必要
                    //ComplexD* result = new ComplexD[N];//数据不需要传出去，这里可以删除，节省 new 和 delete 时间
#pragma omp parallel for//实行批量计算
                    for (int n = 0; n < N_; n++)
                    {
                        double sum_real = 0;
                        double sum_imag = 0;
                        for (int i = 0; i < range_size_; i++)
                        {
                            //这里 ift 操作时虚部需要乘以 -1， 要取负，因为ift 计算出来是 + 虚部， ft_tri_ 是 - 虚部
                            //复数相乘,将虚部和实部先使用变量获取，避免频繁调用函数消磨时间
                            int index = i * N_ + n;
                            const double& ft_real = ft_tri_[index].real;
                            const double& ft_imag = ft_tri_[index].imag;
                            const double& fre_real = frequency_[i].real;
                            const double& fre_imag = frequency_[i].imag;
                            sum_real += (ft_real * fre_real + ft_imag * fre_imag);
                            sum_imag += (ft_real * fre_imag - ft_imag * fre_real);
                        }
                        out[n] = std::sqrt(sum_real * sum_real + sum_imag * sum_imag) * N_inv;
                    }
                }

                //使用 SMID 加速后，速度比 未使用 SMID 的 FFTRun 快大概 100 ms
                void PFFTRun(int N, const Range& range, const double* input, double*& output, bool is_full_spectrum = false) {
                    PFFTInit(N, range, is_full_spectrum);//初始化 fft, 非全谱计算
                    ExecutePFTPlanAVX(input);
                    ExecutePIFTPlan(output);
                }

                //获取到振幅， 相位，幅度，这里必须全谱
                void PFFTRun(int N, const Range& range, const double* input, double*& output, double*& amplitudes,  double*& phases) {
                    PFFTInit(N, range, true);//初始化 fft,全谱计算，并获取到振幅等
                    ExecutePFTPlanAVX(input);//获取到全谱频域

                    //计算出对应的数据
                    const double N_inv = 1.0 / N;
#pragma omp parallel for
                    for (int n = 0; n < N; n++)
                    {
                        const double& out_imag = frequency_[n].imag;
                        const double& out_real = frequency_[n].real;
                        //计算相位
                        // 计算相位（使用atan2处理四象限）
                        double phase = atan2(out_imag, out_real);
                        phases[n] = phase; // [-π, π]
                        //计算振幅
                        double amp = std::sqrt(out_real * out_real + out_imag * out_imag) * N_inv;// 除以 N 进行归一化得到实际值
                        amplitudes[n] = amp;
                        //计算幅度
                        double sum_real = 0;
                        double sum_imag = 0;
                        for (int i = 0; i < range_size_; i++)
                        {
                            int ft_index = i * N_ + n;
                            //这里 ift 操作时虚部需要乘以 -1， 要取负，因为ift 计算出来是 + 虚部， ft_tri_ 是 - 虚部
                            //复数相乘,将虚部和实部先使用变量获取，避免频繁调用函数消磨时间
                            const double& ft_real = ft_tri_[ft_index].real;
                            const double& ft_imag = ft_tri_[ft_index].imag;
                            const double& fre_real = frequency_[i].real;
                            const double& fre_imag = frequency_[i].imag;
                            sum_real += (ft_real * fre_real + ft_imag * fre_imag);
                            sum_imag += (ft_real * fre_imag - ft_imag * fre_real);
                        }
                        output[n] = std::sqrt(sum_real * sum_real + sum_imag * sum_imag) * N_inv;
                    }
                }


                /// <summary>
                /// 根据相位来求得数据高度，相位的斜率与包络图高度是成正比的，看相位获取的高度是否比包络图效果好
                /// 返回斜率
                /// </summary>
                void PFFTPhaseRun(int N, const Range& range, const double* input, double& k, double& b, bool is_full_spectrum = false) {
                    PFFTInit(N, range, is_full_spectrum);//初始化 fft
                    //计算出频域数据，只计算范围内的频域数据,其他都为 0，对于接下来的计算没有意义，所以直接只计算范围内的频域数据，免得后续手动置零了
                    ExecutePFTPlanAVX(input);
                    //根据这段范围内的频域数据，因为我只取对应范围内的数据， 获取到相位数据，
                    auto& m_pool = GetMP();
                    double* phases = m_pool.Allocate<double>(range_size_);//存储振幅数据
                    //振幅计算公式
                    int n = 0;
                    double last_phase = 0;
                    for (int i = 0; i < range_size_; i++)
                    {
                        // 计算相位（使用atan2处理四象限）, -- pmx 这里使用 三角函数，可能会有性能消耗，后续优化使用 SIMD
                        phases[i] = std::atan2(frequency_[i].imag, frequency_[i].real); // [-π, π]
                        //将其绘制成为还原回一条直线
                        if (i != 0)//首位不需要设置
                            if (phases[i] > last_phase) n++;
                        last_phase = phases[i];
                        phases[i] -= n * TwoPi;
                    }

                    //对数据做最小二乘法
                    //计算 Y
                    double Y = 0;
                    for (int i = 0; i < range_size_; i++)
                    {
                        Y += phases[i];
                    }
                    Y /= range_size_;
                    //计算出斜率
                    k = 0;
                    for (int i = 0; i < range_size_; i++)
                    {
                        k += least_squares_sum_x_[i] * (phases[i] - Y);
                    }

                    //做一个计算 b 的处理，实际代码中，这里会被注释掉
                    double X = 0;
                    for (int xi = 0; xi < range_size_; xi++)
                    {
                        X += xi;
                    }
                    X /= range_size_;
                    b = Y - k * X;

                    //释放资源
                    if (phases)
                        m_pool.DeAllocate(phases, range_size_);

                }
            };
        }
    }
}
#endif //PTCAT_PFFT_HPP
