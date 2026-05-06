//
// Created by 27852 on 2026/5/6.
//

#ifndef PTCAT_STATISTICS_PDESCRIPTIVE_HPP
#define PTCAT_STATISTICS_PDESCRIPTIVE_HPP

#include "select/pquicksel.hpp"
#include <execution>
#include "../pmemorypool.hpp"

namespace ptcat {
    namespace palgo {//算法命名空间
        namespace statistics {
            namespace descriptive {//描述性统计
                /**
                 * 获取中位数 - 对于小窗口使用排序，大窗口使用快速选择
                 */
                template<class T>
                inline T GetMedianElement(T* values, int count) {
                    if (count == 1) return values[0];
                    int half_value = count / 2;

                    // 对于小窗口（<= 9），直接使用 std::nth_element 更简单
                    // 当数据量比较小时，算法的开销其实影响并不大，这里可以使用已经搞好的算法，所以这里直接使用排序算法
                    if (count <= 9) {
                        std::nth_element(values, values + half_value, values + count);
                        return values[half_value];
                    }
                    // 对于大窗口，使用快速选择
                    return select::quicksel::QuickSelect(values, count, half_value);
                }

                //获取亚像素峰值的坐标根据加权平均
                template<class T>
                inline
                typename std::enable_if<(std::is_same_v<T, float> || std::is_same_v<T, double>)>::type
                GetPeakIndexByWAvg(T* input, int input_len, int weight,  T& peak_index){
                    //获取到最大值以及其位置
                    //并行运算，这里 C++ 17 及以上， std::execution::par 表示并行计算，加快速度
                    auto max_it = std::max_element(std::execution::par, input, input + input_len);
                    int max_index = std::distance(input, max_it);

                    //获取到最大值，取其两边各 weight_range 个点，进行高斯拟合与加权平均
                    int start = max_index - weight;
                    int end = max_index + weight + 1;
                    if (start > 0 && end < input_len)
                    {
                        //获取到最高点后，进行加权平均求最高点
                        //对于我们数据，如果权重都集中在峰值附近，加权平均会更倾向于该 x 值
                        T d = 0;
                        T n = 0;

                        for (int x = start; x < end; x++)
                        {
                            T y = input[x];
                            n += x * y;
                            d += y;
                        }
                        if (d > std::numeric_limits<T>::epsilon()) {//除 0 保护
                            peak_index = n / d;
                        } else {
                            peak_index = max_index;
                        }
                    }
                    else {
                        peak_index = max_index;
                    }
                }
            }
        }
    }
}

#endif //PTCAT_STATISTICS_PDESCRIPTIVE_HPP
