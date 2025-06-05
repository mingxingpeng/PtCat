//
// Created by 27852 on 2025/6/3.
//

#ifndef PDATE_H
#define PDATE_H

#include "common/common.h"

namespace ptcat {
    namespace datetime {
        class DLL_API PDate {
        protected:
            int year_;
            int month_;
            double day_;

        public:
            PDate();

            PDate(const int year,
                const int month,
                const double day);

            PDate& AddDays(double days);

            PDate& AddMonths(int months);

            PDate& AddYears(int years);

        };
    }
}
#endif //PDATE_H
