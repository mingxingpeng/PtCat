//
// Created by 27852 on 2025/7/10.
//

#ifndef PATTERN_H
#define PATTERN_H
/*
用于设置模板，用于日志的外包装
需要对数据 LogMsg 对象中的数据进行计算，然后计算其对齐，即是在左边加还是再右边加上对应的空格
支持的标志符类型：
    时间相关：%Y（年）、%m（月）、%d（日）、%H（时）、%M（分）、%S（秒）等
    日志级别：%l（完整级别）、%L（简短级别）
    线程/进程：%t（线程ID）、%P（进程ID）
    消息内容：%v（实际日志消息）
    源文件信息：%s（源文件名）、%#（行号）、%!（函数名）
    颜色控制：%^（开始颜色）、%$（结束颜色）
    其他：%n（logger名称）、%%（百分号字符）
 */

#include <string>

namespace ptcat {
    namespace plog {

        // Extract given pad spec (e.g. %8X, %=8X, %-8!X, %8!X, %=8!X, %-8!X, %+8!X)
        // Advance the given it pass the end of the padding spec found (if any)
        // Return padding.
        // SPDLOG_INLINE details::padding_info pattern_formatter::handle_padspec_(
        //     std::string::const_iterator &it, std::string::const_iterator end) {
        //     using details::padding_info;
        //     using details::scoped_padder;
        //     const size_t max_width = 64;
        //     if (it == end) {
        //         return padding_info{};
        //     }
        //
        //     padding_info::pad_side side;
        //     switch (*it) {
        //         case '-':
        //             side = padding_info::pad_side::right;
        //             ++it;
        //             break;
        //         case '=':
        //             side = padding_info::pad_side::center;
        //             ++it;
        //             break;
        //         default:
        //             side = details::padding_info::pad_side::left;
        //             break;
        //     }
        //
        //     if (it == end || !std::isdigit(static_cast<unsigned char>(*it))) {
        //         return padding_info{};  // no padding if no digit found here
        //     }
        //
        //     auto width = static_cast<size_t>(*it) - '0';
        //     for (++it; it != end && std::isdigit(static_cast<unsigned char>(*it)); ++it) {
        //         auto digit = static_cast<size_t>(*it) - '0';
        //         width = width * 10 + digit;
        //     }
        //
        //     // search for the optional truncate marker '!'
        //     bool truncate;
        //     if (it != end && *it == '!') {
        //         truncate = true;
        //         ++it;
        //     } else {
        //         truncate = false;
        //     }
        //     return details::padding_info{std::min<size_t>(width, max_width), side, truncate};
        // }
        //
        // class scoped_padder {
        // public:
        //     scoped_padder(size_t wrapped_size, const padding_info &padinfo, memory_buf_t &dest)
        //         : padinfo_(padinfo),
        //           dest_(dest) {
        //         remaining_pad_ = static_cast<long>(padinfo.width_) - static_cast<long>(wrapped_size);
        //         if (remaining_pad_ <= 0) {
        //             return;
        //         }
        //
        //         if (padinfo_.side_ == padding_info::pad_side::left) {
        //             pad_it(remaining_pad_);
        //             remaining_pad_ = 0;
        //         } else if (padinfo_.side_ == padding_info::pad_side::center) {
        //             auto half_pad = remaining_pad_ / 2;
        //             auto reminder = remaining_pad_ & 1;
        //             pad_it(half_pad);
        //             remaining_pad_ = half_pad + reminder;  // for the right side
        //         }
        //     }
        //
        //     template <typename T>
        //     static unsigned int count_digits(T n) {
        //         return fmt_helper::count_digits(n);
        //     }
        //
        //     ~scoped_padder() {
        //         if (remaining_pad_ >= 0) {
        //             pad_it(remaining_pad_);
        //         } else if (padinfo_.truncate_) {
        //             long new_size = static_cast<long>(dest_.size()) + remaining_pad_;
        //             if (new_size < 0) {
        //                 new_size = 0;
        //             }
        //             dest_.resize(static_cast<size_t>(new_size));
        //         }
        //     }
        //
        // private:
        //     void pad_it(long count) {
        //         fmt_helper::append_string_view(string_view_t(spaces_.data(), static_cast<size_t>(count)),
        //                                        dest_);
        //     }
        //
        //     const padding_info &padinfo_;
        //     memory_buf_t &dest_;
        //     long remaining_pad_;
        //     string_view_t spaces_{"                                                                ", 64};
        // };
        //
        // class Pattern {
        // private:
        //     std::string pattern_;//模板字符串
        //
        // public:
        //     Pattern() = default;
        //     ~Pattern() = default;
        //     Pattern(std::string pattern);
        //
        //     void SetPattern(std::string pattern);//设置模板
        // };
    }
}

#endif //PATTERN_H
