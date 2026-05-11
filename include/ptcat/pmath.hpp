//
// Created by 27852 on 2026/3/16.
//

#ifndef PTCAT_PMATH_HPP
#define PTCAT_PMATH_HPP

#ifndef P_PI
#define P_PI 3.14159265358979323846
#endif

#include <math.h>

static const double HalfOfOne =  1.0 / 2.0;

namespace ptcat{
    namespace palgo {
        constexpr float EPSILON = 1e-10f;

        struct ComplexD {//自己定义复数，减少非必要的对象封装开销
            double real;
            double imag;
        };


        /*
          对于任意的实数 a, mu, sigma，是以著名数学家Carl Friedrich Gauss的名字命名的。
          高斯的一维图是特征对称“bell curve”形状
          a 是曲线尖峰的高度， 一般默认高斯峰的高度是 1.0
          mu 是最高峰的坐标， 也就是尖峰中心的坐标
          sigma 称为标准方差，表征的是bell钟状的宽度，可以用于决定高斯峰权重的宽度
            具体细节看图就明白了：https://blog.csdn.net/qinglongzhan/article/details/82348153
         */
        struct GaussianParam{
            double a_;
            double mu_;
            double sigma_;

            GaussianParam& operator=(const GaussianParam& param) noexcept {
                if(this == &param) return *this;
                this->a_ = param.a_;
                this->mu_ = param.mu_;
                this->sigma_ = param.sigma_;
                return *this;
            }

            bool operator==(const GaussianParam& param){
                if (this == &param) return true;
                return this->a_ == param.a_ && this->mu_ == param.mu_ && this->sigma_ == param.sigma_;
            }
        };

        inline double Gaussian(double x, const GaussianParam& param){
            const double& a = param.a_;
            const double& mu = param.mu_;
            const double& sigma = param.sigma_;
            if (sigma <= 0.0) return 0.0;
            double z = (x - mu) / sigma;
            return a * std::exp(-HalfOfOne * z * z);//这里加了个负号，确保 e 的指数不会很大，设计确实巧妙啊
        }
    }
}
#endif //PTCAT_PMATH_HPP
