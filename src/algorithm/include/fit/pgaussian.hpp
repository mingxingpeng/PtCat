//
// Created by 27852 on 2026/5/6.
//

#ifndef PTCAT_PGAUSSIAN_HPP
#define PTCAT_PGAUSSIAN_HPP
//高斯拟合算法
namespace ptcat{
    namespace palgo {//算法命名空间
        namespace fit {
            namespace gaussian{

//                void GaussianFiter(const double* x_arr, int x_len, const T* y_arr, int y_len, T& k, T& b){
//                    if (!x_arr || !y_arr || x_len != y_len || x_len <= 1) throw std::invalid_argument("invalid parameter");
//                    //进行最小二乘
//                    T sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
//                    for (int i = 0; i < x_len; ++i) {
//                        T x_value = x_arr[i];
//                        T y_value = y_arr[i];
//                        sum_x += x_value;
//                        sum_y += y_value;
//                        sum_xy += x_value * y_value;
//                        sum_x2 += x_value * x_value;
//                    }
//
//                    //防止除 0
//                    const T denominator = (x_len * sum_x2 - sum_x * sum_x);
//                    if (std::fabs(denominator) < T(1e-12)) {
//                        k = 0.0f;
//                        b = sum_y / x_len;
//                        return;
//                    }
//                    k = (x_len * sum_xy - sum_x * sum_y) / denominator;
//                    b = (sum_y - k * sum_x) / x_len;
//                }
            }
        }
    }
}
#endif //PTCAT_PGAUSSIAN_HPP
