//
// Created by Administrator on 2025/3/4.
//
#include "pthreadpool/pthreadpool.h"
#include <iostream>
#include "common/common.h"

namespace ptcat
{
    namespace threadpool
    {
        PThreadPool::PThreadPool() : is_exit_(false), threads_run_count_(0)//使用初始化成员列表为对应参数赋予初始化
        {}

        PThreadPool::~PThreadPool()
        {
            //线程池里的线程如果未销毁需要全部关闭销毁
            if (!threads_.empty() && !is_exit_)
                DestroyThreadPool();
        }

        //创建线程池
        void PThreadPool::CreateThreadPool(const int& threads_size)
        {
            //创建锁
            std::unique_lock<std::mutex> lock(thread_mux_);
            //判断下当前是否已经创建了线程，如果已经创建，提示已经创建了线程池
            if (!threads_.empty())
                throw std::runtime_error("thread pool has been created");
            lock.unlock();

            AddThreads(threads_size);
        }

        //销毁线程池
        void PThreadPool::DestroyThreadPool()
        {
            //先将所有线程池停止
            is_exit_ = true;
            //通知所有线程
            cv_.notify_all();
            //wait_cv_.notify_all();
            //等待所有的线程运行结束
            for (auto& item : threads_)
            {
                if (item->joinable())
                    item->join();
            }

            std::unique_lock<std::mutex> lock(thread_mux_);
            //清除数组
            //如果任务没清空，也清空
            tasks_.clear();
            threads_.clear();
        }

        //添加线程
        void PThreadPool::AddThreads(const int&  thread_size)
        {
            //判断增加的线程池数量是否为 0
            if (thread_size <= 0)
                throw std::runtime_error("the number of thread cannot less than or equal to 0");
            if (thread_size + threads_.size() > 1000)
                throw std::runtime_error("the total number of threads exceeds 1000");

            //创建锁
            std::unique_lock<std::mutex> lock(thread_mux_);
            for (int i = 0; i < thread_size; i++)
            {
                threads_.push_back(std::move(std::make_shared<std::thread>(&PThreadPool::Run, this)));
            }
        }

        //添加需要放置在线程池中处理的任务
        void PThreadPool::AddTask(const std::shared_ptr<task::Task>& task)
        {
            if (!task)
                throw std::runtime_error("the param is nullptr");
            //创建锁之前进行检查
            std::unique_lock<std::mutex> lock(thread_mux_);
            //判断下线程池有没有推出，如果线程池推出了，还添加任务应该是错误的
            if (is_exit_ && threads_.size() <= 0)
                throw std::runtime_error("the current thread pool has exited");

            //如果线程池推出，则对应的死循环线程也应该推出
            task->is_exit_ = [this]()
            {
                return is_exit();
            };
            tasks_.push_back(task);
            lock.unlock();
            cv_.notify_one();//信号通知都一个线程进行处理，必须在解锁之后进行处理
        }

        std::shared_ptr<task::Task> PThreadPool::GetTask()
        {
            std::unique_lock<std::mutex> lock(thread_mux_);
            //如果为空，说明没有任务，就等待
            if (tasks_.empty())
                cv_.wait(lock);
            //如果当前已经正在销毁线程池
            if (is_exit_)
                return nullptr;
            //二次判断一下当前是否已经没有数据
            if (tasks_.empty())
                return nullptr;
            auto task = std::move(tasks_.front());
            tasks_.pop_front();
            //上面都是智能指针，不需要手动释放
            return task;
        }

        //运行函数
        void PThreadPool::Run()
        {
            while (!is_exit_)
            {
                auto task = GetTask();
                if (task)//为 nullptr 不进行处理
                {
                    //加一个 try_catch, 防止一个线程因为传进来的函数导致其他函数也被销毁
                    try
                    {
                        threads_run_count_.fetch_add(1);//去除内存屏障，减少性能损失
                        task->Run();
                    }catch (std::exception e)
                    {
                        std::cerr << std::this_thread::get_id() << " error: " << e.what() << std::endl;
                        break;
                    }
                }
                if (threads_run_count_.load() == 1) {//看是不是最后一个任务
                    threads_run_count_.fetch_sub(1);
                    //notice to stop waiting
                    wait_cv_.notify_one();
                }else {
                    threads_run_count_.fetch_sub(1);
                }

            }
        }

        //waiting for the current tasks to complete
        void PThreadPool::WaitCurrentTask() {
            std::unique_lock<std::mutex> lock(wait_mux_);
            // wait_cv_.wait(lock, [&]() {
            //     return threads_run_count_.load() == 0 || is_exit_;//Check if it returns true, return true and continue to execute downwards
            // });
            wait_cv_.wait(lock);//上面那种写法可能会被后面的谓词影响导致等待失败，wait 一开始为 true,比线程内 threads_run_count_++ 快就会导致这里等待不了直接往下走

        }
    }

}

