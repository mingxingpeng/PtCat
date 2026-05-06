//
// Created by 27852 on 2026/5/6.
//

#include "ptcat/fit/pfit.h"
#include "fit/pleastsquares.hpp"
namespace ptcat {
    namespace palgo {//算法命名空间
        //最小二乘法拟合直线
        void LeastSquaresLinearFiter(const Cat1D<double> x_arr, const Cat1D<double> y_arr, double &k, double &b) {
            fit::leastsquares::LeastSquaresLinearFiter(x_arr.Data(), x_arr.Cols(), y_arr.Data(), y_arr.Cols(), k, b);
        }
        //最小二乘法拟合直线
        void LeastSquaresLinearFiter(const Cat1D<float> x_arr, const Cat1D<float> y_arr, float &k, float &b) {
            fit::leastsquares::LeastSquaresLinearFiter(x_arr.Data(), x_arr.Cols(), y_arr.Data(), y_arr.Cols(), k, b);
        }

    }
}
