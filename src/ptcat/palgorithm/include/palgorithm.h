//
// Created by 27852 on 2026/3/12.
//

#ifndef PTCAT_PALGORITHM_H
#define PTCAT_PALGORITHM_H

#include "ptcat/common/common.h"

namespace ptcat{
    namespace palgo{//算法命名空间
        /**
         * 快速选择算法 - 用于找到中位数
         *      O(n?)	每次都选到最大/最小值作为基准（如已排序数组）
         *      O(n)	每次都能正好选中中位数
         *      如果数值大的话可以优化为 Median of Medians（中位数算法）
         *          他是将数据分组，然后对每一组数据进行排序，然后拿到每一组的中位数， 然后对这几个中位数调用快速选择算法，
         *              就是强行帮忙选到最好基准，然后进行排序
         *  快速选择算法：
         *      快速选择算法是快速排序的变种，用于解决选择问题（找到第 k 小/大的元素）。它的核心思想是：只递归处理包含目标的那一半，而不是像快速排序那样处理全部
         *
         */
        inline float QuickSelect(float* arr, int n, int k) {
            int left = 0, right = n - 1;
            while (left < right) {
                float pivot = arr[(left + right) / 2];
                int i = left, j = right;
                while (i <= j) {
                    while (arr[i] < pivot) i++;
                    while (arr[j] > pivot) j--;
                    if (i <= j) {
                        std::swap(arr[i], arr[j]);
                        i++;
                        j--;
                    }
                }
                if (k <= j) {
                    right = j;
                }
                else if (k >= i) {
                    left = i;
                }
                else {
                    return arr[k];
                }
            }
            return arr[left];
        }


        /**
         * 获取中位数 - 对于小窗口使用排序，大窗口使用快速选择
         */
        float GetMedianElement(float* values, int count) {
            if (count == 1) return values[0];
            int half_value = count / 2;

            // 对于小窗口（<= 9），直接使用 std::nth_element 更简单
            // 当数据量比较小时，算法的开销其实影响并不大，这里可以使用已经搞好的算法，所以这里直接使用排序算法
            if (count <= 9) {
                std::nth_element(values, values + half_value, values + count);
                return values[half_value];
            }

            // 对于大窗口，使用快速选择
            return QuickSelect(values, count, half_value);
        }

    }
}

#endif //PTCAT_PALGORITHM_H
