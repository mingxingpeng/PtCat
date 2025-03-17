//
// Created by Administrator on 2025/3/4.
//

/*
C++ 17 新知识点
    std::apply 是 C++17 引入的一个非常强大的工具，用于将一个可调用对象（如函数、Lambda 表达式、绑定器等）应用于一个元组（std::tuple）中存储的参数。它的主要作用是解包元组中的参数并将它们传递给目标函数。
        std::apply 的定义
            std::apply 的声明位于头文件 <tuple> 中，其定义如下：
                template <class F, class Tuple>
                constexpr decltype(auto) apply(F&& f, Tuple&& t);
                    F&& f：目标可调用对象（如函数、Lambda 表达式等）。
                    Tuple&& t：一个元组对象，存储了目标函数的参数。
                    返回值：目标函数的返回值。
        std::apply 的工作原理
            std::apply 的实现基于递归模板展开和完美转发。它的主要步骤如下：
                解包元组：通过递归模板展开，将元组中的每个元素解包。
                完美转发：使用 std::forward 将解包后的参数以正确的值类别传递给目标函数。

        举例：
            template <typename Function, typename... Args>
            void callFunction(Function&& func, Args&&... args)
            {
                auto paramsa = std::make_tuple(std::forward<Args>(args)...);
                std::apply(std::forward<Function>(func), std::move(paramsa));
            }
 */
/*
 发现了一些不常见的问题，对于注释符号 // 如果后面有中文，可能会出错，以后在遇到的记着这一茬
 */

#ifndef PTHREADPOOL_H
#define PTHREADPOOL_H

//针对 windows 下静态库动态库不一致需要这样操作
#ifdef _WIN32
    #ifdef STATIC_LIB//对于 windows 静态库，也不需要指定
        #define DLL_API
    #else//对于 windows 动态库，如果是对应库文件，需要指定 __declspec(dllexport)，非库文件指定 __declspec(dllimport)
        #ifdef PThreadPool_EXPORTS//调用 add_libraries 才会有这个 xxx_EXPORTS 这个宏生成,单个单个是生成 PThreadPool 这个库
            #define DLL_API __declspec(dllexport)//windows 系统下对于动态库，需要指定 __declspec(dllexport)，不然会导致动态库 .lib 函数地址文件生成不了
        #else
            #ifdef PtCat_EXPORTS//如果是集体生成，会生成 PtCat 这个库
                #define DLL_API __declspec(dllexport)//windows 系统下对于动态库，需要指定 __declspec(dllexport)，不然会导致动态库 .lib 函数地址文件生成不了
            #else
                #define DLL_API __declspec(dllimport)
            #endif
        #endif
    #endif
#else//linux 不需要
    #define DLL_API
#endif


#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <list>
#include "ptask.h"
#include <memory>
#include <condition_variable>

/*
线程池，包含功能
    开启指定数量的线程
    调用线程池中的线程
    关闭线程池
    增加线程池数量

    author : pmx
    time :   2025/3/4
 */

namespace ptcat//定义命名空间
{
    class DLL_API PThreadPool final {
    public:
        PThreadPool();
        ~PThreadPool();

        PThreadPool(const PThreadPool& p) = delete;
        PThreadPool& operator=(const PThreadPool&& p) = delete;

        //创建线程池
        void CreateThreadPool(const int& threads_size);

        //销毁线程池
        void DestroyThreadPool();

        //线程池中添加线程
        void AddThreads(const int&  thread_size);

        //添加任务
        void AddTask(const std::shared_ptr<Task>& task);

        //返回正在运行的线程数量， 函数代码量比较小，直接 inline 函数
        int threads_run_count() const
        {
            return threads_run_count_.load();
        }
        //判断是否退出
        bool is_exit() const
        {
            return is_exit_;
        }
    private:
        //用于存储线程
        std::vector<std::shared_ptr<std::thread>> threads_;
        //判断是否推出线程池
        bool is_exit_;
        //运行线程数量
        std::atomic_int threads_run_count_;
        //锁
        std::mutex thread_mux_;

        std::list<std::shared_ptr<Task>> tasks_;//需要被放置在线程中处理的任务

        std::condition_variable cv_;//条件变量

        void Run();//运行函数

        std::shared_ptr<Task> GetTask();//获取到任务进行处理
    };
}

#endif //PTHREADPOOL_H
