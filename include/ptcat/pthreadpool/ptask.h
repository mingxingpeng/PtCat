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
#include <functional>
#include <memory>


namespace ptcat//定义命名空间
{
    namespace task
    {
        typedef std::function<bool()> ISEXIT;

        //使用 C++ 17 来代替 C++ 20 的功能，不使用 C++20,下面功能是用于限制当前模板类中函数的第一个函数只能是 ISEXIT 类型
        template <typename Function, typename... Args>
        using is_invocable = typename std::is_invocable<Function, ISEXIT, Args...>::type;//std::is_invocable 判断函数是否符合运行规则以及 传入的参数是否满足函数的要求

        class Task
        {
        public:
            ISEXIT is_exit_ = nullptr;//设置是否推出
            virtual void Run() = 0;

        protected:
            Task() = default;

            //设置为虚析构，防止为父类传递子类对象，然后调用父类调用函数析构时不调用子类析构函数
            virtual ~Task()= default;
        };

        /*
         *定义任务类
         */
        template<typename Function, typename... Args>
        //requires std::is_invocable_v<Function, ISEXIT, Args...> /*requires: C++20 新功能，这里的作用是确保函数第一个参数一定是指定类型函数指针*/
        class PTask final : public Task
        {
        public:

            // 使用静态断言确保函数可调用
            //static_assert(is_invocable<Function, Args...>::value, "the first parameter of the function must be of type ISEXIT.");

            /*使用列表表达式的方式成员变量赋值， std::tuple<Args...> args_ 获取参数包中的参数使用 std::make_tuple(std::forward<Args>(args)...) 方式获取*/
            explicit PTask(Function&& func, Args&&... args) : func_(std::forward<Function>(func)), args_(std::forward_as_tuple(std::forward<Args>(args)...))//使用 forward_as_tuple 在生成 tuple 时不会修改类型，而 make_tuple 会
            {
            }

            ~PTask() = default;

            PTask(const PTask&) = delete;
            PTask(const PTask&&) = delete;
            PTask& operator=(const PTask&) = delete;
            PTask& operator=(const PTask&&) = delete;

            void Run() override
            {
                /*调用apply 函数进行对元组进行解包以及调用该函数*/
                auto tuple = std::tuple_cat(std::make_tuple(std::forward<ISEXIT>(is_exit_)), std::forward<std::tuple<Args...>>(args_));
                std::apply(func_, std::move(tuple));
            }
        private:
            Function func_;//需要进行处理的函数
            std::tuple<Args...> args_;//需要进行处理的函数的参数元组
        };


        //创建一个任务指针
        template<typename Function, typename... Args>
        auto make_task(Function&& func, Args&&... args)
        {
            static_assert(is_invocable<Function, Args...>::value, "the first parameter of the function must be of type ISEXIT.");
            return std::make_shared<ptcat::task::PTask<Function, Args...>>(std::forward<Function>(func), std::forward<Args>(args)...);
        }

    }
}
#endif //PTASK_H
