//
// Created by 27852 on 2025/6/2.
//

#ifndef PCLOCKHELPER_H
#define PCLOCKHELPER_H
#include <map>
#include "ptcat/common/common.h"
#include <cmath>

namespace ptcat {
    enum class DateTimeType {//时间枚举
        YEAR,
        MONTH,
        DAY,
        HOUR,
        MINUTE,
        SECOND,
        MILLISECOND
    };

    const int LeapYear2MonthDayCount = 29;
    const int CommonYear2MonthDayCount = 28;
    const int MaxMonthCount = 12;
    const int MinMonthCount = 1;
    const std::map<int, int> CommonYearMonthDays = {{1, 31},
                                            {2, 28},
                                            {3, 31},
                                            {4, 30},
                                            {5, 31},
                                            {6, 30},
                                            {7, 31},
                                            {8, 31},
                                            {9, 30},
                                            {10, 31},
                                            {11, 30},
                                            {12, 31}};
    const double Epsilon = 1e-8;//表示double 类型小数部分被允许的误差范围，因为 double

    ///Determine whether it is a leap year
    inline bool IsLeapYear(const int& year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    ///Get the number of days in a certain year and month
    inline void GetMonthCount(const int& year, const int& month, int& day_count) {
        if (IsLeapYear(year)) {
            if (month == 2) {
                day_count = LeapYear2MonthDayCount;
                return;
            }
        }
        auto it = CommonYearMonthDays.find(month);
        if (it == CommonYearMonthDays.end()) {
            day_count = KeyNotFound;
        }else {
            day_count = it->second;
        }
    }

    ///How many days in total are obtained in a year
    inline void GetTotalDaysInYear(const int& year, int& day_count) {
        day_count = 0;
        for (auto month: CommonYearMonthDays) {
            if (month.first == 2) {
                if (IsLeapYear(year))
                    day_count += LeapYear2MonthDayCount;
                else
                    day_count += CommonYear2MonthDayCount;
            }else {
                day_count += month.second;
            }
        }
    }

    ///How many days of the year is a certain day obtained
    // inline void GetCertainDayNumInYear(const int& year, const int& month, const int& day, int& day_count) {
    //
    // }
    //
    ///How many weeks of the year is obtained for a certain week
    // inline void GetCertainWeekNumInYear(const int& year, const int& month, int& day_count) {
    //
    // }


    //实现对时分秒的计算
    inline void AddateTime(const DateTimeType& add_type, const double& add_count, double& year, double& month, double& day, double& hour, double& minute, double& second, double& milli_second) {
        




        if (add_type == DateTimeType::YEAR) {
            year += add_count;
            //判断一下是否有小数
            double integer;
            double decimal = std::modf(year, &integer);//std::modf 获取到小数以及整数
            if (decimal > Epsilon) {//表示小数有值
                double new_month = decimal* MaxMonthCount + month;
                if (new_month > MaxMonthCount) {
                    //计算出需要加上几年
                    int add_year = new_month / MaxMonthCount;
                    year += add_year;
                    month = new_month - add_year * MaxMonthCount;
                    decimal = std::modf(month, &integer);
                    if (decimal > Epsilon) {

                    }
                }
            }
        }


        if(add_type == DateTimeType::DAY) {
            int new_add_count = add_count;//向上取整
            if (add_count > 0) {
                while (true) {
                    int day_count = 0;
                    GetMonthCount(year, month, day_count);
                    day += new_add_count;
                    if (day <= day_count) break;
                    new_add_count = day - day_count;
                    day = 0;
                    if (month == MaxMonthCount) {
                        month = MinMonthCount;
                        year += 1;
                    }else {
                        month += 1;
                    }
                }
            }else {
                while (true) {
                    day += new_add_count;
                    if (day > 0) break;
                    new_add_count = std::abs(day);
                    if (month == MinMonthCount) {
                        month = MaxMonthCount;
                        year -= 1;
                    }else {
                        month -= 1;
                    }
                    int day_count = 0;
                    GetMonthCount(year, month, day_count);
                    day = day_count;
                }
            }
            //
        }else if (add_type == DateTimeType::MONTH) {
            if (add_count > 0) {
                int new_add_count = std::ceil(add_count);//向上取整
                while (true) {
                    month += add_count;
                    if (month <= MaxMonthCount) break;
                    new_add_count = month - MaxMonthCount;
                    month = 0;
                    year += 1;
                }
            }else {
                int new_add_count = std::ceil(std::abs(add_count));//向上取整
                while (true) {
                    month -= add_count;
                    if (month >= MinMonthCount) break;
                    new_add_count = std::abs(month);
                    month = MaxMonthCount;
                    year -= 1;
                }
            }

        }else if (add_type == DateTimeType::YEAR) {
            year += add_count;
        }
    }

}

#endif //PCLOCKHELPER_H
