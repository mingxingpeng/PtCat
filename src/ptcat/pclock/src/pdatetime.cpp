//
// Created by 27852 on 2025/5/27.
//

#include "ptcat/pclock/pdatetime.h"
#include "pclockhelper.h"


namespace ptcat {
    namespace datetime {

        PDateTime::PDateTime()  : year_{0},
                                month_{0},
                                day_{0},
                                hour_{0},
                                minute_{0},
                                second_{0},
                                milli_second_{0} ,
                                day_of_week_{0},
                                day_of_year_{0}{}

        PDateTime::PDateTime(const int year,
                        const int month,
                        const double day) : year_{year},
                                        month_{month},
                                        day_{day},
                                        hour_{0},
                                        minute_{0},
                                        second_{0},
                                        milli_second_{0} ,
                                        day_of_week_{0},
                                        day_of_year_{0}{}
         PDateTime::PDateTime(const int year,
            const int month,
            const double day,
            const double hour,
            const double minute,
            const double second) : year_{year},
                                month_{month},
                                day_{day},
                                hour_{hour},
                                minute_{minute},
                                second_{second},
                                milli_second_{0} ,
                                day_of_week_{0},
                                day_of_year_{0}{}
         PDateTime::PDateTime(const int year,
            const int month,
            const double day,
            const double hour,
            const double minute,
            const double second,
            const double milli_second) : year_{year},
                                month_{month},
                                day_{day},
                                hour_{hour},
                                minute_{minute},
                                second_{second},
                                milli_second_{milli_second} ,
                                day_of_week_{0},
                                day_of_year_{0}{}

        //get current time
        PDateTime PDateTime::Now() {
            PDateTime p_date_time;
            PSystemClock::time_point current_time = PSystemClock::now();
            //得到毫秒
            /*
            time_since_epoch()
                表示从 1970 年到现在的总时间，我们只需要获取到这个时间的总毫秒数，然后取模 1000， 获取到当前时间的毫秒数即可
             */
            auto ms = PChrono::duration_cast<PChrono::milliseconds>(current_time.time_since_epoch()) % 1000;
            p_date_time.milli_second_ = static_cast<int>(ms.count());
            //获取到年月日时分秒
            std::time_t t = PSystemClock::to_time_t(current_time);//将时间转换为 time_t

            std::tm local_tm;

#if defined(_WIN32) || defined(_WIN64)
            localtime_s(&local_tm, &t);
#else
            localtime_r(&t, &local_tm);
#endif

            p_date_time.year_ = local_tm.tm_year + 1900;
            p_date_time.month_ = local_tm.tm_mon + 1;
            p_date_time.day_ = local_tm.tm_mday;
            p_date_time.hour_ = local_tm.tm_hour;
            p_date_time.minute_ = local_tm.tm_min;
            p_date_time.second_ = local_tm.tm_sec;
            return p_date_time;//std::move(p_date_time) ，不需要这样写，因为 C++ 编译器会自动做返回值优化，这样写反而会影响优化
        }

        /*
         年：闰年 366， 平年 365
         月: 1 31， 2 28/29， 3 31， 4 30， 5 31， 6 30， 7 31， 8 31， 9 30， 10 31， 11 30， 12 31

         */
        PDateTime& PDateTime::AddDays(double days) {
            this->day_ += days;
            return *this;
        }

        PDateTime& PDateTime::AddHours(double hours) {
            this->hour_ += hours;
            return *this;
        }

        PDateTime& PDateTime::AddMinutes(double minutes) {
            this->minute_ += minutes;
            return *this;
        }

        PDateTime& PDateTime::AddSeconds(double seconds) {
            this->second_ += seconds;
            return *this;
        }

        PDateTime& PDateTime::AddMilliseconds(double milliseconds) {
            this->milli_second_ += milliseconds;
            return *this;
        }

        PDateTime& PDateTime::AddMonths(int months) {
            this->month_ += months;
            return *this;
        }

        PDateTime& PDateTime::AddYears(int years) {
            this->year_ += years;
            return *this;
        }

    }
}
