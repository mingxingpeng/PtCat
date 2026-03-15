//
// Created by 27852 on 2026/3/13.
//

#ifndef PTCAT_FILTER_CORE_H
#define PTCAT_FILTER_CORE_H


#include "../palgorithm.h"
#include "../pmemorypool.h"
#include <algorithm>
#ifdef _OPENMP
#include <omp.h>
#endif

namespace ptcat {
    namespace palgo {//算法命名空间
        namespace core{
            //中值滤波， 数据为空就直接报错
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
                            *curr_data = GetMedianElement(sort_data, sort_real_count);
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
#endif //PTCAT_FILTER_CORE_H
