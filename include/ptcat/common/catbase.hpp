//
// Created by 27852 on 2026/3/20.
//

#ifndef PTCAT_CATBASE_HPP
#define PTCAT_CATBASE_HPP
#include "ptcat/common/common.h"
#include <atomic>

namespace ptcat{
    template<class T>
    class CatBase{
    protected:
        int cols_;//数据列
        int rows_;//数据行
        int dims_;//数据维度，一般是 2 维
        std::atomic_int* ref_count_;//引用计数
        T* data_ = nullptr;//用于存储数据
        bool is_external_data = false;//用于判断是不是外部数据

        void AddRef(){
            if (ref_count_)
                //使用 memory_order_acq_rel 确保内存屏障
                ref_count_->fetch_add(1, std::memory_order_acq_rel);
        }

        bool SubRef(){
            if (ref_count_){
                //使用 memory_order_acq_rel 确保内存屏障
                return ref_count_->fetch_sub(1, std::memory_order_acq_rel) == 1;//fetch_sub 返回的是减去之前的值
            }else {
                return false;
            }
        }

        void SetDefault(){
            this->data_ = nullptr;
            this->ref_count_ = nullptr;
            this->rows_ = 0;
            this->cols_ = 0;
            this->dims_ = 0;
            this->is_external_data = false;
        }
    };
}


#endif //PTCAT_CATBASE_HPP
