//
// Created by Administrator on 2025/3/6.
//

#ifndef PTASK_H
#define PTASK_H
/*
    task class
    author : pmx
    time   : 2025/3/6
 */
#include <tuple>


namespace ptcat//定义命名空间
{
    class Task
    {
    public:
        Task() = default;

        //设置为虚析构，防止为父类传递子类对象，然后调用父类调用函数析构时不调用子类析构函数
        virtual ~Task()= default;

        virtual void Run() = 0;
    };

    /*
     *定义任务类
     */
    template<typename Function, typename... Args>
    class PTask : public Task
    {
    public:
        /*使用列表表达式的方式成员变量赋值， std::tuple<Args...> args_ 获取参数包中的参数使用 std::make_tuple(std::forward<Args>(args)...) 方式获取*/
        PTask(Function&& func, Args&&... args) : func_(std::forward<Function>(func)), args_(std::make_tuple(std::forward<Args>(args)...))
        {
        }

        ~PTask() = default;

        void Run() override
        {
            /*调用apply 函数进行对元组进行解包以及调用该函数*/
            std::apply(std::forward<Function>(func_), std::move(args_));
        }
    private:
        Function func_;//需要进行处理的函数
        std::tuple<Args...> args_;//需要进行处理的函数的参数元组
    };
}
#endif //PTASK_H
