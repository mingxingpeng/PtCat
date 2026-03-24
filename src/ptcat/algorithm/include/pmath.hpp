//
// Created by 27852 on 2026/3/16.
//

#ifndef PTCAT_PMATH_HPP
#define PTCAT_PMATH_HPP

#ifndef P_PI
#define P_PI 3.14159265358979323846
#endif


namespace ptcat{
    namespace palgo {
        constexpr float EPSILON = 1e-10f;

        struct ComplexD {//自己定义复数，减少非必要的对象封装开销
            double real;
            double imag;
        };
    }
}
#endif //PTCAT_PMATH_HPP
