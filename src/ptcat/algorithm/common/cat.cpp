//
// Created by 27852 on 2026/3/20.
//

#include "ptcat/common/cat.h"

namespace ptcat {
    template<class T>
    Cat<T>::Cat(){
        SetDefault();
    }

    template<class T>
    //使用外部数据
    Cat<T>::Cat(int rows, int cols, T* data){
        this->rows_ = rows;
        this->cols_ = cols;
        this->dims_ = (rows == 1 || cols == 1) ? 1 : 2;
        this->data_ = data;
        this->ref_count_ = nullptr;//外部数据也不分配引用计数，因为他不是我们想要的数据
        this->is_external_data = true;
    }

    template<class T>
    Cat<T>::Cat(int rows, int cols){
        this->rows_ = rows;
        this->cols_ = cols;
        this->dims_ = (rows == 1 || cols == 1) ? 1 : 2;
        this->data_ = new T[cols * rows];
        this->ref_count_ = new std::atomic_int(1);//外部数据也不分配引用计数，因为他不是我们想要的数据
        this->is_external_data = false;
    }

    template<class T>
    Cat<T>::~Cat(){
        Clear();
    }

    template<class T>
    //拷贝函数
    Cat<T>::Cat(const Cat<T>& cat){
        this->data_ = cat.data_;
        this->cols_ = cat.cols_;
        this->rows_ = cat.rows_;
        this->dims_ = cat.dims_;
        this->is_external_data = cat.is_external_data;
        //引用技术 ++
        this->ref_count_ = cat.ref_count_;
        AddRef();
    }

    template<class T>
    Cat<T>& Cat<T>::operator=(const Cat<T>& cat){
        if (&cat == this) return;
        //释放当前资源
        Clear();
        //复制新数据
        this->data_ = cat.data_;
        this->cols_ = cat.cols_;
        this->rows_ = cat.rows_;
        this->dims_ = cat.dims_;
        this->is_external_data = cat.is_external_data;
        //引用技术 ++
        this->ref_count_ = cat.ref_count_;
        AddRef();
    }

    template<class T>
    Cat<T>::Cat(Cat<T>&& cat) noexcept{
        //复制新数据
        this->data_ = cat.data_;
        this->cols_ = cat.cols_;
        this->rows_ = cat.rows_;
        this->dims_ = cat.dims_;
        this->is_external_data = cat.is_external_data;
        //引用技术 ++
        this->ref_count_ = cat.ref_count_;

        //去除源对象的控制权
        cat.data_ = nullptr;
        cat.ref_count_ = nullptr;
        cat.rows_ = cat.cols_ = cat.dims_ = 0;
        cat.is_external_data = false;
    }

    template<class T>
    Cat<T>& Cat<T>::operator=(Cat<T>&& cat) noexcept{
        //释放当前资源
        Clear();
        //复制新数据
        this->data_ = cat.data_;
        this->cols_ = cat.cols_;
        this->rows_ = cat.rows_;
        this->dims_ = cat.dims_;
        this->is_external_data = cat.is_external_data;
        //引用技术 ++
        this->ref_count_ = cat.ref_count_;
        //去除源对象的控制权
        cat.data_ = nullptr;
        cat.ref_count_ = nullptr;
        cat.rows_ = cat.cols_ = cat.dims_ = 0;
        cat.is_external_data = false;
    }


    //清除数据
    template<class T>
    void Cat<T>::Clear(){
        if (!this->is_external_data && this->ref_count_)//如果时内部数据，才删除
        {
            if (SubRef())
                if (this->data_){
                    delete [] this->data_;
                    delete this->ref_count_;
                }
        }
        SetDefault();
    }
}