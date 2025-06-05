//
// Created by 27852 on 2025/6/3.
//

#include "pclock/pdate.h"
#include "pclockhelper.h"//help

namespace ptcat {
    namespace datetime {
        PDate::PDate()  : year_{0},
                            month_{0},
                            day_{0}{}

         PDate::PDate(const int year,
                        const int month,
                        const double day): year_{year},
                                month_{month},
                                day_{day}{}

        PDate&  PDate::AddDays(double days) {

        }

        PDate&  PDate::AddMonths(int months) {

        }

        PDate&  PDate::AddYears(int years) {

        }
    }
}