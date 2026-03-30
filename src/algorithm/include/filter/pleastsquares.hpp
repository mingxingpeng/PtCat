//
// Created by 27852 on 2026/3/30.
//
//最小二乘的算法
#ifndef PTCAT_LEASTSQUARES_H
#define PTCAT_LEASTSQUARES_H

#include <stdexcept>
#include <cmath>

namespace ptcat{
    namespace palgo {//算法命名空间
        namespace leastsquares {
            ///最小二乘拟合直线
            /*
            求线性拟合，根据公式 y = kx + b, 根据最小二乘法的定律，当样本值减去实际值的平方和达到最小时，这个时候的样本数据与原始数据最为接近，也就是就 yi - (kxi + b) 的平方和求其极值，数学上求极值通常是对其求导并且令导数等于0.
            最小二乘法公式：
                n						n
                ∑	(yi - f(xi))^2  =>  ∑ (yi - (kxi + b))^2
                i=0						i=0
                对其求偏导并让其导数为 0(导数为 0 表示梯度到达最低点，即与原数据最相近点，在该点，所有偏导数为 0， 然后这里因为数值比较简单，只有一个最优解，且特征单一，直接让其导数为 0 得到最优结果，而不需要使用梯度下降去判断)，然后得到 k 与 b：
                              n∑xiyi - ∑xi∑yi			      ∑yi - k∑xi
                    得到 k = ----------------------		b = --------------
                              n∑(xi^2) - (∑xi)^2				  n

                    下面是优化的方式，懒得搞了，自己研究吧
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
            template<class T>
            inline
            typename std::enable_if<(std::is_same_v<T, double> || std::is_same_v<T, float>)>::type
            LeastSquaresLinearFiter(const T* x_arr, int x_len, const T* y_arr, int y_len, T& k, T& b){
                if (!x_arr || !y_arr || x_len != y_len || x_len <= 1) throw std::invalid_argument("invalid parameter");
                //进行最小二乘
                T sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
                for (int i = 0; i < x_len; ++i) {
                    T x_value = x_arr[i];
                    T y_value = y_arr[i];
                    sum_x += x_value;
                    sum_y += y_value;
                    sum_xy += x_value * y_value;
                    sum_x2 += x_value * x_value;
                }

                //防止除 0
                const T denominator = (x_len * sum_x2 - sum_x * sum_x);
                if (std::fabs(denominator) < T(1e-12)) {
                    k = 0.0f;
                    b = sum_y / x_len;
                    return;
                }
                k = (x_len * sum_xy - sum_x * sum_y) / denominator;
                b = (sum_y - k * sum_x) / x_len;
            }
        }
    }
}
#endif //PTCAT_LEASTSQUARES_H
