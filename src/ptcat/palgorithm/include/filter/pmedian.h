//
// Created by 27852 on 2026/3/16.
//

#ifndef PTCAT_PMEDIAN_H
#define PTCAT_PMEDIAN_H

#include "../select/pquicksel.h"
#include "../pmemorypool.h"
#ifdef _OPENMP
#include <omp.h>
#endif

namespace ptcat {
    namespace palgo {//算法命名空间
        namespace median{
            //中值滤波， 数据为空就直接报错
            /*
            中值滤波原理：
                中值滤波是一种非线性的数字滤波技术，核心思想非常简单直观：
                    用邻域内所有像素值的中位数来代替当前像素的值
            工作原理
                定义邻域窗口：以当前像素为中心，定义一个固定大小的窗口
                        收集像素值：获取窗口内所有像素的数值
                        排序取中：将这些值排序，取中间位置的值
                        替换原值：用这个中位数替换当前像素的值
            为什么有效？
                椒盐噪声（随机黑白点）通常表现为极端的最大值或最小值，排序后这些极端值会跑到两端，中位数不会被它们影响
                        边缘保留：中位数能很好地保留边缘信息，因为边缘两侧的像素值不会互相平均
             */

            template<class T>
            void MediaFilter(T* data, int height, int width, int kernel){
                if (!data || height <= 0 || width <= 0 || kernel <= 0) throw std::invalid_argument("invalid parameter");
                auto& mpool = GetMP();

                if (kernel % 2 == 0)//确保核大小为奇数
                    kernel++;
                int radius = kernel / 2;//获取到核半径
                //获取到临时数据用于排序
                int kernel_size = kernel * kernel;

                //将原数据拷贝一份用于进行卷积
                int data_size = height * width;
                T* temp = mpool.Allocate<T>(data_size);

                std::copy(data, data + data_size, temp);
                //遍历数据
#pragma omp parallel for//加上后速度从 900 多ms 降低到 168 ms
                for (int row = 0; row < height; row++){
                    T* curr_data = data + row * width;//获取到当前行数据第一个索引

                    T* sort_data = mpool.Allocate<T>(kernel_size);//因为我现在使用并行处理，所以这个内存获取每一个线程开启一个内存

                    for (int col = 0; col < width; col++){
                        int sort_real_count = 0;//将实际数据数量置为 0
                        //获取到核内的所有数据，然后进行排序
                        //提前获取到范围
                        int start_row = std::max(0, row - radius);
                        int end_row = std::min(height - 1, row + radius);
                        int start_col = std::max(0, col - radius);
                        int end_col = std::min(width - 1, col + radius);

                        for (int k_row = start_row; k_row <= end_row; k_row++){
                            for (int k_col = start_col; k_col <= end_col; k_col++){
                                int index = k_row * width + k_col;
                                sort_data[sort_real_count++] = *(temp + index);
                            }
                        }
                        //对数据进行排序
                        if (sort_real_count > 0){
                            //修改为当前中值，否则保持原值
                            *curr_data = quicksel::GetMedianElement(sort_data, sort_real_count);
                        }
                        //遍历到下一个数据
                        curr_data++;
                    }
                    mpool.DeAllocate(sort_data, kernel_size);
                }
                mpool.DeAllocate(temp, data_size);
            }
        }
    }
}

#endif //PTCAT_PMEDIAN_H
