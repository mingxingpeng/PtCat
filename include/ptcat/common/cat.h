//
// Created by 27852 on 2026/3/20.
//

#ifndef PTCAT_CAT_H
#define PTCAT_CAT_H

#include "catbase.hpp"

namespace ptcat {
    template <typename T>
    //容器类
    class DLL_API Cat final : public CatBase<T>{
    public:
        Cat();

        Cat(int rows, int cols, T* data);

        Cat(int rows, int cols);

        ~Cat();

        Cat(const Cat& cat);//拷贝构造器

        Cat<T>& operator=(const Cat& cat);

        Cat(Cat&& cat) noexcept;

        Cat<T>& operator=(Cat&& cat) noexcept;

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
            return Data()[index];
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

        void Clear();
    };
}
#endif //PTCAT_CAT_H
