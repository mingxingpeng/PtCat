//
// Created by 27852 on 2025/5/27.
//

#ifndef PDATETIME_H
#define PDATETIME_H
/// datetime lib
/*
 *在 C# 中，DateTime 是一个结构体（struct），用于表示日期和时间。它提供了丰富的功能来处理日期和时间相关的操作。以下是 DateTime 类的主要组成部分和功能：
1. 构造函数
DateTime 提供了多个构造函数，用于创建日期和时间的实例：
csharp
复制
DateTime yearMonthDay = new DateTime(2025, 5, 27); // 2025年5月27日
DateTime yearMonthDayHourMinuteSecond = new DateTime(2025, 5, 27, 14, 30, 45); // 2025年5月27日14:30:45
DateTime yearMonthDayHourMinuteSecondMillisecond = new DateTime(2025, 5, 27, 14, 30, 45, 123); // 2025年5月27日14:30:45.123
2. 属性
DateTime 提供了许多属性，用于获取日期和时间的各个部分：
年份：Year
月份：Month
日期：Day
小时：Hour
分钟：Minute
秒：Second
毫秒：Millisecond
星期几：DayOfWeek
一年中的第几天：DayOfYear
时间的 Kind（本地时间、UTC 时间等）：Kind
示例：
csharp
复制
DateTime now = DateTime.Now;
Console.WriteLine($"Year: {now.Year}, Month: {now.Month}, Day: {now.Day}");
Console.WriteLine($"Hour: {now.Hour}, Minute: {now.Minute}, Second: {now.Second}");
Console.WriteLine($"Day of Week: {now.DayOfWeek}, Day of Year: {now.DayOfYear}");
3. 方法
DateTime 提供了许多方法，用于日期和时间的计算、格式化和比较：
加减日期和时间：
AddDays(double days)：添加天数
AddHours(double hours)：添加小时数
AddMinutes(double minutes)：添加分钟数
AddSeconds(double seconds)：添加秒数
AddMilliseconds(double milliseconds)：添加毫秒数
AddMonths(int months)：添加月数
AddYears(int years)：添加年数
示例：
csharp
复制
DateTime now = DateTime.Now;
DateTime tomorrow = now.AddDays(1);
DateTime nextYear = now.AddYears(1);
比较日期和时间：
CompareTo(DateTime value)：比较两个 DateTime 实例
Equals(DateTime value)：判断两个 DateTime 实例是否相等
示例：
csharp
复制
DateTime now = DateTime.Now;
DateTime tomorrow = now.AddDays(1);

Console.WriteLine(now.CompareTo(tomorrow)); // 输出 -1
Console.WriteLine(now.Equals(tomorrow)); // 输出 False
格式化日期和时间：
ToString()：将 DateTime 转换为字符串，支持自定义格式化
ToString(string format)：使用指定的格式化字符串
示例：
csharp
复制
DateTime now = DateTime.Now;
Console.WriteLine(now.ToString()); // 默认格式
Console.WriteLine(now.ToString("yyyy-MM-dd HH:mm:ss")); // 自定义格式
获取当前日期和时间：
DateTime.Now：获取当前本地时间
DateTime.UtcNow：获取当前 UTC 时间
DateTime.Today：获取当前日期（时间部分为 00:00:00）
示例：
csharp
复制
DateTime now = DateTime.Now;
DateTime utcNow = DateTime.UtcNow;
DateTime today = DateTime.Today;

Console.WriteLine(now);
Console.WriteLine(utcNow);
Console.WriteLine(today);
4. 静态方法
DateTime 还提供了一些静态方法，用于日期和时间的计算：
解析字符串为日期和时间：
Parse(string s)：将字符串解析为 DateTime 实例
TryParse(string s, out DateTime result)：尝试解析字符串，返回布尔值表示是否成功
示例：
csharp
复制
DateTime date = DateTime.Parse("2025-05-27");
bool success = DateTime.TryParse("2025-05-27", out DateTime parsedDate);
获取特定日期和时间：
MinValue：表示最小可能的日期和时间（0001-01-01 00:00:00）
MaxValue：表示最大可能的日期和时间（9999-12-31 23:59:59.9999999）
示例：
csharp
复制
Console.WriteLine(DateTime.MinValue);
Console.WriteLine(DateTime.MaxValue);
5. 时间间隔
DateTime 与 TimeSpan 结合使用，可以方便地处理时间间隔：
csharp
复制
DateTime start = DateTime.Now;
// 模拟一些操作
Thread.Sleep(1000);
DateTime end = DateTime.Now;

TimeSpan duration = end - start;
Console.WriteLine($"Duration: {duration.TotalSeconds} seconds");
总结
DateTime 是 C# 中处理日期和时间的核心类，提供了丰富的属性和方法，可以满足大多数日期和时间相关的操作需求。通过合理使用这些功能，可以方便地进行日期和时间的计算、格式化和比较。
 *
 */

#include "ptcat/common/common.h"

namespace ptcat {
    namespace datetime {

        class DLL_API PDateTime {
        private:
            int year_;
            int month_;
            double day_;
            double hour_;
            double minute_;
            double second_;
            double milli_second_;
            double day_of_week_;
            double day_of_year_;
        public:
            PDateTime();

            PDateTime(const int year,
                const int month,
                const double day);
            PDateTime(const int year,
                const int month,
                const double day,
                const double hour,
                const double minute,
                const double second);
            PDateTime(const int year,
                const int month,
                const double day,
                const double hour,
                const double minute,
                const double second,
                const double milli_second);

            static PDateTime Now();

            PDateTime& AddDays(double days);

            PDateTime& AddHours(double hours);

            PDateTime& AddMinutes(double minutes);

            PDateTime& AddSeconds(double seconds);

            PDateTime& AddMilliseconds(double milliseconds);

            PDateTime& AddMonths(int months);

            PDateTime& AddYears(int years);


        };
    }
}




#endif //PDATETIME_H
