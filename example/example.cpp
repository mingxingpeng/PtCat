#include <iostream>
#include <tuple>
#include "pthreadpool.h"
#include "plogwirter.h"
#include <functional>

void TestPthreadPool();
void TestPLogWriter();


int main()
{

    TestPLogWriter();
    TestPthreadPool();

    return 0;
}

void TestPthreadPool()
{
    //pthreadpool
    ptcat::threadpool::PThreadPool pool;
    //create thread pool and specify the number of thread
    pool.CreateThreadPool(10);
    //get run thread count
    int num = pool.threads_run_count();
    std::cout << "threads_run_count " << num << std::endl;
    // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    //create task
    pool.AddTask(ptcat::task::make_task([](ptcat::task::ISEXIT is_exit, std::string str)
    {
        while(true)
        {
            if (is_exit())
                break;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            std::cout << str << std::endl;
        }
    }, "i miss you"));

    pool.AddTask(ptcat::task::make_task([](ptcat::task::ISEXIT is_exit, int num, std::string str)
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

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //pool.DestroyThreadPool();
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        pool.AddTask(ptcat::task::make_task([](ptcat::task::ISEXIT is_exit, int num, std::string str)
        {

               //std::this_thread::sleep_for(std::chrono::milliseconds(num));
               std::cout << num << "  " << str << std::endl;
        }, 10, "i test you,you just want to add it -------------------------------------"));

    }
    std::cout << "hello world " << std::endl;
}

void TestPLogWriter()
{
    PLogWriter plog;
}