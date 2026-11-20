//
// Created by 27852 on 2026/3/13.
//

/*
================================================================================
              离散傅里叶变换、包络提取与相位斜率测高 -- 完整数学推导
================================================================================

【零、全篇数学符号与参数定义】

1. 基础时域与频域参数
     N          : 离散信号总采样点数 (正整数)
     n          : 时域(或空间域)采样点索引，n ∈ {0, 1, ..., N-1}
     x[n]       : 在位置 n 采集到的原始实数信号值
     k          : 频率序号(频域索引)，k ∈ {0, 1, ..., N-1}
                  物理含义: 在 N 个采样点内，标准简谐波完整震荡了 k 个周期
                    把 k = 0, 1, 2, 500, 999 公式中的 cos(2π * k * n / 1000) 这一步为例，就不使用全部了
                        直接看每个 k 算出来的 1000 个数长啥样
                            k = 0：公式变成 cos(0)算出来的 1000 个数是：[+1, +1, +1, +1, ..., +1]
                            k = 1：n 从 0 走到 999，角度刚好慢慢转 1 圈， 算出来的 1000 个数是：[+1 ... 慢慢降到 0 ... 第500个点到 -1 ... 慢慢升回 0 ... 第999个点回到 +1]
                            k = 2：n 从 0 走到 999，角度转 2 圈。算出来的 1000 个数是：[+1 -> -1 -> +1 -> -1 -> +1]（正负来回翻 2 趟）。用来测你的数据里有没有“上下起伏 2 次”的大波浪。
                            k = 500（最极端的情况）：公式变成 cos(π * n)。n=0 是 +1，n=1 是 -1，n=2 是 +1，n=3 是 -1……算出来的 1000 个数是：[+1, -1, +1, -1, +1, -1, ..., +1, -1]紧挨着的两个点一正一负疯狂切换！它是用来测你的数据是不是每走一步都在剧烈上下乱跳。
                            k = 999（：想象一块表盘，n 每加 1，往顺时针走 999/1000 圈，落点和往逆时针倒退 1/1000 圈一模一样！因为 cos(-x) == cos(x)，所以 k = 999 算出来的 1000 个 cos 值和 k = 1 完全一模一样（只是 sin 变了个负号）。
                            同理，k = 998 是 k = 2 的镜像，k = 501 是 k = 499 的镜像。对实数输入来说，501 ~ 999 只是 499 ~ 1 的对称拷贝。
                    傅里叶变换中频率序号 k 与计算模式
                        k 的核心本质（我觉得就是穷举出我这个数据所有可能的震荡形态， 然后这些震荡形态叠加起来就是长度为 N 的复杂曲线）
                            对于长度为 N（如 N = 1000）的离散数据，k 是用来穷举所有可能震荡快慢的编号（取值 0 -> N-1）：
                                穷举性：从完全不震荡到震荡得最剧烈，这 N 个 k 囊括了 N 个采样点内所有可能存在的震荡形态。
                                叠加性：任何一条长度为 N 的复杂曲线，本质上都是由这些不同 k 的基础震荡叠加拼出来的。
                                筛选原理（内积）：每个 k 对应一套长度为 N 的标准波模板（cos 与 -sin）。将原始数据与模板逐点相乘再累加--节奏对得上就同号放大，对不上就
                                正负抵消趋近于 0。
                        不同 k 值的直观含义（以 N = 1000 为例）
                            k 的取值                   模板特征（晃动快慢）                  物理/数据含义
                            k = 0                      全为 +1，完全不震荡               直流分量（数据总和 / 平均亮度底色）
                            k 较小（如 1, 2）             缓慢起伏，晃动次数少               低频成分（整体倾斜、背景缓慢漂移）
                            k 居中某段                   疏密适中的规则波浪                有效目标震荡信号（核心提取区域）
                            k = N/2（500）              +1, -1 紧挨着交替，震荡最急        最高频成分（相邻点剧烈跳动、毛刺噪点）
                            N/2+1 -> N-1（501->999）     倒序对应的负频率                 实信号前半段（499->1）的对称镜像

                        代码中的两种计算模式闭环
                            全谱模式：把 0 ~ N-1 所有震荡情况全部算一遍，用于完整获取振幅谱与相位谱，摸清数据里到底混了哪些成分。
                            局部模式（指定范围）：已知有效信号只落在某一段震荡快慢内，只挑这一段 k 计算，其余无关频段直接当 0 跳过，同时实现去噪滤波与大幅提速。
     j          : 虚数单位，满足 j^2 = -1
     θ(k, n)    : 旋转相角，定义为 θ(k, n) = 2π*k*n / N
     c(k, n)    : 正变换旋转因子的实部，c(k, n) =  cos(θ(k, n))
     d(k, n)    : 正变换旋转因子的虚部，d(k, n) = -sin(θ(k, n))
     X[k]       : 向量数据，第 k 个频点的复数频谱，X[k] = R[k] + j*I[k]
     R[k], I[k] : 频域复数 X[k] 的实部 Re(X[k]) 与虚部 Im(X[k])
     A[k]       : 第 k 个频点的归一化振幅谱 (反映该频率成分的强弱)
     φ_wrap[k]  : 第 k 个频点的包裹相位(主值相位)，取值范围 [-π, π]

2. 频域滤波与包络提取参数
     [k1, k2]   : 有效正频率带通区间，区间长度(频点数) M = k2 - k1 + 1
     G[k]       : 频域滤波窗函数权重 (不加窗时恒为 1)
     a, μ, σ    : 高斯窗的峰值幅值(a=1)、中心频率 μ 与标准差 σ (σ > 0)
     X~[k]      : 频域加窗滤波后的复数频谱，X~[k] = R~[k] + j*I~[k]
     z[n]       : 单边带逆变换重构的复数解析信号，z[n] = zr[n] + j*zi[n]
     E[n]       : 时域包络信号 (去除高频载波震荡后的瞬时幅度轮廓)

3. 相位解包裹与最小二乘拟合参数
     q          : 相位解包裹累计跳变周期数 (整数)
     φ[k]       : 解包裹后还原的连续展开相位
     m          : 有效频段内的局部相对序号，m ∈ {0, 1, ..., M-1}，对应 k = k1 + m
     u_m, v_m   : 直线拟合的横坐标 (u_m = m) 与纵坐标 (v_m = φ[k1 + m])
     u_bar      : 横坐标 u_m 的算术平均值
     v_bar      : 纵坐标 v_m 的算术平均值
     w_m        : 仅与横坐标相关的最小二乘预计算权重系数
     K, B       : 拟合直线 v = K*u + B 的斜率 K 与截距 B
     n0         : 信号包络峰值对应的真实时域(或扫描)位置
     H, C       : 最终换算出的物理高度(或位移) H 及比例换算系数 C

--------------------------------------------------------------------------------
【一、离散傅里叶正变换 (DFT)：时域 -> 频域】
1. 标准定义
         将实数时域信号 x[n] 投影到频率为 k 的复指数正交基上:
         (全谱时 X 是长度为 N 的复数向量，实部和虚部拆开存放占 2*N 个 double；
          局部频段 [k1, k2] 时长度为 M，拆开存放占 2*M 个 double)
                        N-1
             X[k]  =    ∑   x[n] * e^(-j * 2π*k*n / N)
                       n=0

2. 上面公式使用欧拉公式展开 (复数拆分为实部与虚部)
         令 θ(k, n) = 2π*k*n / N，由欧拉公式 e^(-j*θ(k, n)) = cos(θ(k, n)) - j*sin(θ(k, n)):
             c(k, n) =  cos(θ(k, n))  // 固定 k、遍历 n∈[0, N-1] 时为长度 N 的向量 (实部分量)
             d(k, n) = -sin(θ(k, n))  // 固定 k、遍历 n∈[0, N-1] 时为长度 N 的向量 (虚部分量)

         因为 cos 和 sin 计算比较耗时，所以初始化时根据有效频段 [k1, k2] 提前算好并存放起来
         (代码中按 [cos, -sin, cos, -sin, ...] 交错存在同一个指针数组 ft_tri_ 中，
          每个频点 k 占 2*N 个 double)，供下面计算频域时直接查表使用。

         代入定义式，得到每个频点 k 的实部与虚部:
                        N-1                         N-1
             R[k]  =    ∑   x[n] * c(k, n),    I[k] = ∑   x[n] * d(k, n)
                       n=0                         n=0
            ==>
             X[k]  = R[k] + j * I[k]

         物理直觉: 对每一个固定频率 k，用长度同为 N 的余弦波 c(k, n) 和正弦波 d(k, n)
                  与信号 x[n] 做逐点相乘再累加(求内积)，把 N 个时域点浓缩为 1 个复数 X[k]。
                  当仅关注有效频段 [k1, k2] 时，只需对 k ∈ [k1, k2] 计算局部 DFT，
                  其余频点视作 0，不影响后续带通分析。
--------------------------------------------------------------------------------
【二、频域带通与高斯加窗滤波】
    对信号在频域施加带通滤波，以压制直流偏置、低频漂移与高频噪声:
1. 高斯窗函数定义
             G[k] = a * exp( -(k - μ)^2 / (2*σ^2) )   (通常取峰值 a = 1)
         其中中心频率取有效区间中点:  μ = k1 + (k2 - k1) / 2。
2. 7σ 截断近似
         当 |k - μ| = 7σ 时，G[k] = exp(-49/2) = exp(-24.5) ≈ 2.3 × 10^(-11) ≈ 0。
         因此可将高斯滤波的实际有效频段截断为:
             [k1', k2'] = [ max(0, μ - 7σ),  min(N - 1, μ + 7σ) ]
3. 频域加权乘法 (等价于时域卷积滤波)
         这里相当于给指定频段的频域数据乘上一个平滑过渡的高斯权重，
         而不是直接“一刀切”做硬截断(频段外全置 0、内部全为 1)。
         这样逆变换还原出的时域包络会更平滑，避免硬截断在主峰旁引入多余波纹或异常突起(振铃效应):
             X~[k] = G[k] * X[k]
                   =>  R~[k] = G[k] * R[k]
                       I~[k] = G[k] * I[k]
         (若不启用高斯窗，则直接取 G[k] = 1，即矩形窗理想带通滤波: X~[k] = X[k]。)
--------------------------------------------------------------------------------
【三、全谱特征提取：振幅谱、相位谱与对称重排】
    在复平面上，每个频点 X[k] = (R[k], I[k]) 对应一个长度为 2 的向量，其模长和辐角分别决定
    该频率分量的强弱与初始相位:
1. 归一化振幅谱
         A[k] = (1 / N) * √( R[k]^2 + I[k]^2 )
            (注意：除以 N 用于抵消 DFT 中 N 点累加带来的幅值放大，还原真实量级。)
2. 包裹相位谱 (四象限反正切)
         φ_wrap[k] = atan2( I[k], R[k] )   ∈ [-π, π]
3. 频谱对称重排 (将零频移至中心)
     标准 DFT 输出顺序为: [ 0频(直流), 正频率, 负频率 ]。
     设 h = floor(N / 2)，重排为以零频为中心的对称顺序 [ 负频率, 0频(直流), 正频率 ]:
- 当 N 为偶数时:
           新[0]          = 原[h]              (奈奎斯特频率)
           新[1 .. h-1]   = 原[h+1 .. N-1]     (负频率段)
           新[h]          = 原[0]              (直流分量)
           新[h+1 .. N-1] = 原[1 .. h-1]       (正频率段)
- 当 N 为奇数时:
           新[0 .. h-1]   = 原[h+1 .. N-1]     (负频率段)
           新[h]          = 原[0]              (直流分量)
           新[h+1 .. N-1] = 原[1 .. h]         (正频率段)
--------------------------------------------------------------------------------
【四、单边带逆变换 (IDFT) 与时域包络提取】
1. 物理原理
     典型的载波调制实信号可表示为 x[n] = E[n] * cos(ω0*n + φ0)，其频谱在正频率 +ω0
     与负频率 -ω0 处呈共轭对称双峰。若只保留正频率有效频段 [k1, k2] 做逆傅里叶变换
     (丢弃负频率与带外噪声)，重构出的不再是实数震荡波，而是复数解析信号:
         z[n] ∝ E[n] * e^(+j*(ω0*n + φ0))
     对该复数取模长，旋转因子 |e^(+j*(ω0*n + φ0))| ≡ 1 被完全消除，仅留下光滑包络 E[n]。
2. 逆变换公式及旋转因子共轭复用推导
     标准逆离散傅里叶变换(IDFT)指数项为正号:
                    1     k2
         z[n]  =  ----- * ∑  X~[k] * e^(+j * θ(k, n))
                    N    k=k1

     其中 e^(+j*θ(k, n)) = cos(θ(k, n)) + j*sin(θ(k, n))。
     因为正变换中已定义 c(k, n) = cos(θ(k, n))，d(k, n) = -sin(θ(k, n))，
     所以逆变换旋转因子恰好是正变换旋转因子的复共轭:
         e^(+j*θ(k, n)) = c(k, n) - j*d(k, n)
     将其与滤波后频谱 X~[k] = R~[k] + j*I~[k] 展开相乘:
         (R~[k] + j*I~[k]) * (c(k, n) - j*d(k, n))
           = ( R~[k]*c(k, n) + I~[k]*d(k, n) )
           + j * ( I~[k]*c(k, n) - R~[k]*d(k, n) )
     对区间内所有频点 k ∈ [k1, k2] 求和，得复解析信号的实部与虚部(未归一化):
                     k2
         zr[n]  =    ∑   ( c(k, n)*R~[k] + d(k, n)*I~[k] )
                    k=k1
                     k2
         zi[n]  =    ∑   ( c(k, n)*I~[k] - d(k, n)*R~[k] )
                    k=k1
3. 取模得到时域包络
         E[n] = (1 / N) * √( zr[n]^2 + zi[n]^2 )

     (注: 因单边带只取了实信号正负频谱中的一半能量，严格还原物理幅值需乘 2/N；
          乘 1/N 不改变包络曲线形状与峰值位置。)
================================================================================
*/

#include "ptcat/algorithm/fft/pfft.h"
#include "../include/pmemorypool.hpp"
#include <fstream>

namespace ptcat {
    namespace palgo {//算法命名空间
        namespace fft{
            const static double TwoPi = 2 * P_PI;

            static PMemoryPool& MPool = GetMP();

            PFFT::PFFT() : is_use_full_spectrum_(false),
                           variable_range_({0, 0}),
                           variable_range_size_(0),
                           envelope_range_({0, 0}),
                           envelope_range_size_(0),
                           ft_tri_(nullptr),
                           ft_size_(0),
                           N_(0),
                           is_use_gaussian_(false),
                           gaussian_weight_(nullptr),
                           gaussian_param_({0, 0, 0}),
                           gaussian_weight_size_(0)
            {}

            PFFT::~PFFT(){
                PFFTDeInit();
            }

            //初始化所需资源
            void PFFT::PFFTInit(const int& N, const Range& range, bool is_full_spectrum, bool is_gaussian, int sigma) {
                //为类变量赋值
                if (N <= 0) throw std::invalid_argument("invalid argument");
                bool is_same = (N_ == N) && (envelope_range_ == range)
                        && (is_use_full_spectrum_ == is_full_spectrum)
                            && (is_use_gaussian_ == is_gaussian);
                N_ = N;
                variable_range_ = range;
                envelope_range_ = range;

                //查看是否使用全谱
                is_use_full_spectrum_ = is_full_spectrum;

                //判断是否使用高斯，如果使用高斯的话
                is_use_gaussian_ = is_gaussian;
                GaussianParam gaussian_param;
                if (is_use_gaussian_){
                    //先计算一下参数值
                    gaussian_param.a_ = 1.0;
                    int half_range_size = (envelope_range_.end - envelope_range_.start) * 0.5;
                    gaussian_param.mu_ = envelope_range_.start + half_range_size ;
                    gaussian_param.sigma_ = sigma;
                    //高斯不一定要使用全谱，因为因为 7 * sigma + mu_ 这个位置的数据已经可以看成零了,我看大部分分都为 e-11，已经是很小的数了
                    //这个时候只需要将后续需要计算的范围设置为 mu_ 前后 7 * sigma， 超出范围则设置为边界值
                    int seven_sigma = 7 * sigma;
                    int gauss_start = gaussian_param.mu_ - seven_sigma;
                    int gauss_end = gaussian_param.mu_ + seven_sigma;
                    envelope_range_.start = gauss_start > 0 ? gauss_start : 0;
                    envelope_range_.end = gauss_end < N_ - 1 ? gauss_end : N_ - 1;
                    //需要将实际范围与他设置的一致
                    variable_range_ = envelope_range_;
                }
                //如果使用全谱就设置下 variable_range_ 的值
                if (is_use_full_spectrum_){
                    variable_range_.start = 0;
                    variable_range_.end = N_ - 1;
                }

                //计算指定区域得傅里叶变换复数，或者全局傅里叶变换复数，也就是说是否计算全谱傅里叶变换得复数
                variable_range_size_ = variable_range_.end - variable_range_.start + 1;
                envelope_range_size_ = envelope_range_.end - envelope_range_.start + 1;

                //如果参数更新，更新数据
                if (is_use_gaussian_ && (!gaussian_weight_ || gaussian_weight_size_ != variable_range_size_ || !(gaussian_param == gaussian_param_))){
                    if (gaussian_weight_)
                        MPool.DeAllocate(gaussian_weight_, gaussian_weight_size_);//删除上一个数据得数据长度
                    gaussian_weight_size_ = variable_range_size_;//gaussian_weight_size_ 主要用于记录之前的长度
                    gaussian_param_ = gaussian_param;
                    gaussian_weight_ = MPool.Allocate<double>(gaussian_weight_size_);//申请内存
                    //非全谱就使用指定范围的，否则就是用全部的
                    for (int gi = 0; gi < gaussian_weight_size_; ++gi) {
                        int gk = variable_range_.start + gi;
                        gaussian_weight_[gi] = Gaussian(gk, gaussian_param);
                    }
                }

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
                }
            }

            //释放资源
            void PFFT::PFFTDeInit() {
                if (ft_tri_)
                {
                    MPool.DeAllocate(ft_tri_, ft_size_);
                    ft_tri_ = nullptr;
                }
                if (gaussian_weight_){
                    MPool.DeAllocate(gaussian_weight_, gaussian_weight_size_);
                    gaussian_weight_ = nullptr;
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
                    //统一改频率，目前先不在下面反 fft 操作这一步了
                    double freq_weight = 1.0;
                    if (is_use_gaussian_)
                        freq_weight = gaussian_weight_[i];
                    frequency[fre_i] = freq_weight * total_real;
                    frequency[fre_i + 1] = freq_weight * total_imag;
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
            void PFFTN::PFFTRun(const double* input, double*& output, double*& amplitudes,  double*& phases, bool reorder_to_symmetric) {
                //判断下是不是全谱
                if (!is_use_full_spectrum_) throw std::runtime_error("please set it to full spectrum during initialization");
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
                    for (int i = 0; i < envelope_range_size_; i++)
                    {
                        int calc_index = envelope_range_.start + i;
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
                if (reorder_to_symmetric){//之前相位和振幅是标准排序，下面转换为对称排序
                    double* temp = MPool.Allocate<double>(N_);
                    int half_N = N_ * 0.5;
                    memcpy(temp, amplitudes, N_ * sizeof(double));//做数据备份
                    if (N_ % 2 == 0)//偶数
                    {
                        amplitudes[0] = temp[half_N];//Nquist 数据放开头
                        //DC 数据放在中间
                        amplitudes[half_N] = temp[0];
                        //拷贝负频率
                        memcpy(amplitudes + 1, temp + half_N + 1, (half_N - 1) * sizeof(double));//做数据备份
                        //拷贝正频率
                        memcpy(amplitudes + half_N + 1, temp + 1, (half_N - 1) * sizeof(double));//做数据备份
                    }
                    else {//奇数
                        //DC 放中间
                        amplitudes[half_N] = temp[0];
                        //拷贝负频率
                        memcpy(amplitudes, temp + half_N + 1, half_N * sizeof(double));//做数据备份
                        //拷贝正频率
                        memcpy(amplitudes + half_N + 1, temp + 1, half_N * sizeof(double));//做数据备份
                    }
                    //相位
                    memcpy(temp, phases, N_ * sizeof(double));//做数据备份
                    if (N_ % 2 == 0)//偶数
                    {
                        phases[0] = temp[half_N];//Nquist 数据放开头
                        //DC 数据放在中间
                        phases[half_N] = temp[0];
                        //拷贝负频率
                        memcpy(phases + 1, temp + half_N + 1, (half_N - 1) * sizeof(double));//做数据备份
                        //拷贝正频率
                        memcpy(phases + half_N + 1, temp + 1, (half_N - 1) * sizeof(double));//做数据备份
                    }
                    else {//奇数
                        //DC 放中间
                        phases[half_N] = temp[0];
                        //拷贝负频率
                        memcpy(phases, temp + half_N + 1, half_N * sizeof(double));//做数据备份
                        //拷贝正频率
                        memcpy(phases + half_N + 1, temp + 1, half_N * sizeof(double));//做数据备份
                    }
                    MPool.DeAllocate(temp, N_);
                }
                FreeFrequency(frequency);
            }
        }
    }
}
