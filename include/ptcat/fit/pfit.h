//
// Created by 27852 on 2026/5/6.
//

#ifndef PTCAT_PFIT_H
#define PTCAT_PFIT_H
#include "ptcat/common/common.h"
#include <memory>
#include "ptcat/common/cat.hpp"

namespace ptcat {
    namespace palgo {//Ëã·¨ÃüÃû¿Õ¼ä
        DLL_API void LeastSquaresLinearFiter(const Cat1D<double> x_arr, const Cat1D<double> y_arr, double &k, double &b);

        DLL_API void LeastSquaresLinearFiter(const Cat1D<float> x_arr, const Cat1D<float> y_arr, float &k, float &b);


    }
}

#endif //PTCAT_PFIT_H
