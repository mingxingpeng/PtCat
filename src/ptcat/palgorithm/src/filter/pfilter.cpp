//
// Created by 27852 on 2026/3/13.
//

#include "ptcat/palgorithm/filter/pfilter.h"
#include <iostream>
#include "pmemorypool.h"
#include <algorithm>
#ifdef _OPENMP
#include <omp.h>
#endif

/**
 * 快速选择算法 - 用于找到中位数（平均 O(n) 复杂度）
 */
float quickSelect(float* arr, int n, int k) {
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
float getMedian(float* values, int count) {
    if (count == 1) return values[0];

    // 对于小窗口（<= 9），直接使用 std::nth_element 更简单
    if (count <= 9) {
        std::nth_element(values, values + count / 2, values + count);
        return values[count / 2];
    }

    // 对于大窗口，使用快速选择
    return quickSelect(values, count, count / 2);
}


namespace ptcat{
    namespace palgo{//算法命名空间


        //中值滤波
        void MediaFilter(double* data, int height, int width, int kernel){

        }


        //中值滤波， 数据为空就直接报错
        void MediaFilter(float* data, int height, int width, int kernel){
            if (!data || height <= 0 || width <= 0 || kernel <= 0) throw std::invalid_argument("invalid parameter");
            auto& mpool = GetMP();

            if (kernel % 2 == 0)//确保核大小为奇数
                kernel++;
            int radius = kernel / 2;//获取到核半径
            //获取到临时数据用于排序
            int kernel_size = kernel * kernel;

            //将原数据拷贝一份用于进行卷积
            int data_size = height * width;
            float* temp = mpool.Allocate<float>(data_size);

            std::copy(data, data + data_size, temp);
            //遍历数据
            #pragma omp parallel for//加上后速度从 900 多ms 降低到 168 ms
            for (int row = 0; row < height; row++){
                float* curr_data = data + row * width;//获取到当前行数据第一个索引

                float* sort_data = mpool.Allocate<float>(kernel_size);//因为我现在使用并行处理，所以这个内存获取每一个线程开启一个内存

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
                        *curr_data = getMedian(sort_data, sort_real_count);
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
