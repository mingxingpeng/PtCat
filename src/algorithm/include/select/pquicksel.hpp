//
// Created by 27852 on 2026/3/16.
//

#ifndef PTCAT_SELECT_PQUICKSEL_HPP
#define PTCAT_SELECT_PQUICKSEL_HPP

#include <algorithm>
#include <stdexcept>

namespace ptcat {
    namespace palgo {//算法命名空间
        namespace select{
            namespace quicksel{
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
                //k_index 是索引，找出第 k+1 小的数据
                template<class T>
                inline T QuickSelect(T* arr, int count, int k_index){
                    // 1. 核心边界校验：避免越界访问
                    if (!arr || count <= 0 || k_index < 0 || k_index >= count) {
                        throw std::invalid_argument("Invalid arguments in QuickSelect");
                    }
                    int left = 0, right = count - 1;//获取到分区界限
                    //根据当前分区进行进行排序
                    while(left < right){
                        int i = left, j = right;
                        /*
                        如果中间是极值，就会导致循环多几次，所以使用三位数取中来避开极值
                         */
                        //取当前分组的中间值作为基准值
                        //T pivot = arr[(left + right) / 2];
                        //优化基准值的初始值，三位数取中选择基准值
                        int mid = left + (right - left) / 2;
                        if (arr[left] > arr[mid]) std::swap(arr[left], arr[mid]);
                        if (arr[mid] > arr[right]) std::swap(arr[mid], arr[right]);
                        if (arr[left] > arr[mid]) std::swap(arr[left], arr[mid]);
                        T pivot = arr[mid];

                        //将大于基准值的值移到右边，小于基准值的值移到左边
                        while(i <= j){
                            while(arr[i] < pivot) i++;//对于小于基准值的数据，不需要移动，否则需要移动到基准值的右边,即找到左边第一个 >= pivot 的元素
                            while(arr[j] > pivot) j--;//对于大于基准值的数据，不需要移动，否则需要移动到基准值的左边,即找到右边第一个 <= pivot 的元素
                            //对数据进行移动，即小于基准值的数据移动到左边，大于基准值的数据移动到右边
                            if (i <= j){
                                std::swap(arr[i], arr[j]);
                                i++;
                                j--;
                            }
                        }
                        //重新进行分组，然后重复上面的步骤
                        if (k_index <= j)//如果需要找的索引小于 j, 说明需要寻找的目标数据在左边，将右边的界限索引更新为 j
                            right = j;
                        else if(k_index >= i)//如果需要找的索引大于 i, 说明需要寻找的目标数据在右边，将左边的界限索引更新为 i
                            left = i;
                        else
                            return arr[k_index];
                    }
                    return arr[left];
                }
            }
        }
    }
}

#endif //PTCAT_SELECT_PQUICKSEL_HPP
