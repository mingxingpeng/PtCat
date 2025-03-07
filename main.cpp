#include <iostream>
#include <tuple>
#include "pthreadpool.h"
#include <functional>



int main()
{
    //pthreadpool
    ptcat::PThreadPool pool;
    //create thread pool and specify the number of thread
    pool.CreateThreadPool(10);
    //get run thread count
    int num = pool.threads_run_count();
    std::cout << "threads_run_count " << num << std::endl;
    // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    //create task
    pool.AddTask(std::make_shared<ptcat::PTask<std::function<void(ptcat::ISEXIT, std::string)>, std::string>>([](ptcat::ISEXIT is_exit, std::string str)
    {
        while(true)
        {
            if (is_exit())
                break;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            std::cout << str << std::endl;
        }
    }, "i miss you"));

    pool.AddTask(std::make_shared<ptcat::PTask<std::function<void(ptcat::ISEXIT, int, std::string)>, int, std::string>>([](ptcat::ISEXIT is_exit, int num, std::string str)
   {
       while(true)
       {
           if (is_exit())
               break;
           std::this_thread::sleep_for(std::chrono::milliseconds(1));
           std::cout << num << "  " << str << std::endl;
       }
   }, 100, "i miss you"));

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    num = pool.threads_run_count();
    std::cout << "------------- threads_run_count " << num << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    // pool.DestroyThreadPool();
    // while(true)
    // {
    //     std::this_thread::sleep_for(std::chrono::milliseconds(1));
    // }

    return 0;
}
