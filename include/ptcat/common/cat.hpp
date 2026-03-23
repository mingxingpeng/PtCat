//
// Created by 27852 on 2026/3/20.
//

#ifndef PTCAT_CAT_HPP
#define PTCAT_CAT_HPP

#include "ptcat/common/common.h"
#include <iostream>

#include <atomic>
namespace ptcat {

    template <typename T>
    //容器类
    class CatBase {
    protected:
        int cols_;//数据列
        int rows_;//数据行
        int dims_;//数据维度，一般是 2 维
        std::atomic_int* ref_count_;//引用计数
        T* data_ = nullptr;//用于存储数据
        bool is_external_data = false;//用于判断是不是外部数据

        virtual ~CatBase(){
            Clear();
        }

        void SetDefault(){
            this->data_ = nullptr;
            this->ref_count_ = nullptr;
            this->rows_ = 0;
            this->cols_ = 0;
            this->dims_ = 0;
            this->is_external_data = false;
        }

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

    public:
        int Size() const {
            return this->rows_ * this->cols_;
        }

        //查看指针数据
        T* Data(){
            return this->data_;
        }

        const T* Data() const{
            return this->data_;
        }

        //根据索引获取数据
        T& At(int index) {
            return Data()[index];//d294541065
        }

        //根据索引获取数据
        const T& At(int index) const{
            return Data()[index];
        }

        //根据索引获取数据
        T& operator[](int index){
            return Data()[index];
        }

        const T& operator[](int index) const{
            return Data()[index];
        }

        void CopyWith(const CatBase<T>& cat){
            this->data_ = cat.data_;
            this->cols_ = cat.cols_;
            this->rows_ = cat.rows_;
            this->dims_ = cat.dims_;
            this->is_external_data = cat.is_external_data;
            //引用技术 ++
            this->ref_count_ = cat.ref_count_;
            AddRef();
        }

        void MoveWith(CatBase<T>&& cat){
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

        int Rows() const {
            return rows_;
        }

        int Cols() const {
            return cols_;
        }

        //判断数据是否为空
        bool Empty() const {
            return data_ == nullptr || rows_ == 0 || cols_ == 0;
        }

        void Clear(){
            if (!this->is_external_data && this->ref_count_)//如果时内部数据，才删除
            {
                if (SubRef())
                    if (this->data_){
                        std::cout << "delete cat data: " <<  this->data_ << std::endl;
                        delete [] this->data_;
                        delete this->ref_count_;
                    }
            }
            SetDefault();
        }
    };

    template <typename T>
    //容器类
    class Cat final : public CatBase<T>{
    private:

    public:
        Cat(){
            SetDefault();
        }

        //使用外部数据
        Cat(int rows, int cols, T* data){
            this->rows_ = rows;
            this->cols_ = cols;
            this->dims_ = (rows == 1 || cols == 1) ? 1 : 2;
            this->data_ = data;
            this->ref_count_ = nullptr;//外部数据也不分配引用计数，因为他不是我们想要的数据
            this->is_external_data = true;
        }

        Cat(int rows, int cols){
            this->rows_ = rows;
            this->cols_ = cols;
            this->dims_ = (rows == 1 || cols == 1) ? 1 : 2;
            this->data_ = new T[cols * rows];
            this->ref_count_ = new std::atomic_int(1);//外部数据也不分配引用计数，因为他不是我们想要的数据
            this->is_external_data = false;
        }

        ~Cat(){

        }

        Cat(const Cat<T>& cat){//拷贝构造器
            CopyWith(cat);
        }

        Cat<T>& operator=(const Cat<T>& cat){
            if (&cat == this) return *this;
            //释放当前资源
            Clear();
            CopyWith(cat);
            return *this;
        }

        Cat(Cat<T>&& cat) noexcept{
            MoveWith(std::move(cat));
        }

        Cat<T>& operator=(Cat<T>&& cat) noexcept{
            if (&cat == this) return *this;
            //释放当前资源
            Clear();
            MoveWith(std::move(cat));
            return *this;
        }
    };

    template <typename T>
    //容器类
    class Cat1D final : public CatBase<T>{
    private:

    public:
        Cat1D(){
            SetDefault();
        }

        //使用外部数据
        Cat1D(int cols, T* data){
            this->rows_ = 1;
            this->cols_ = cols;
            this->dims_ = 1;
            this->data_ = data;
            this->ref_count_ = nullptr;//外部数据也不分配引用计数，因为他不是我们想要的数据
            this->is_external_data = true;
        }

        Cat1D(int cols){
            this->rows_ = 1;
            this->cols_ = cols;
            this->dims_ = 1;
            this->data_ = new T[cols];
            this->ref_count_ = new std::atomic_int(1);//外部数据也不分配引用计数，因为他不是我们想要的数据
            this->is_external_data = false;
        }

        ~Cat1D(){

        }

        Cat1D(const Cat1D<T>& cat){//拷贝构造器
            CopyWith(cat);
        }

        Cat1D<T>& operator=(const Cat1D<T>& cat){
            if (&cat == this) return *this;
            //这里可能会面临指针数据一样的情况，那么下面就会删除对应指针数据，会导致问题，但是一般不太可能会出现，以后在改 pmx
            //释放当前资源
            Clear();
            CopyWith(cat);
            return *this;
        }

        Cat1D(Cat1D<T>&& cat) noexcept{
            MoveWith(std::move(cat));
        }

        Cat1D<T>& operator=(Cat1D<T>&& cat) noexcept{
            if (&cat == this) return *this;
            //释放当前资源
            Clear();
            MoveWith(std::move(cat));
            return *this;
        }
    };
}
#endif //PTCAT_CAT_HPP
