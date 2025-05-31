#include <iostream>
#include <tuple>
#include "pthreadpool/pthreadpool.h"
#include "plogwriter/logger/plogwirter.h"
#include <functional>
#include "pclock/pstopwatch.h"

void TestClock();
void TestPthreadPool();
void TestPLogWriter();



int main()
{
    // TestClock();
    // TestPLogWriter();
    TestPthreadPool();

    return 0;
}

void TestClock(){
    ptcat::clock::StopWatch sw;
    sw.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    sw.Stop();
    std::cout << "MicroElapsedTime = " << sw.MicroElapsedTime() << " μs" << std::endl;
    std::cout << "MilliElapsedTime = " << sw.MilliElapsedTime() << " ms" << std::endl;
    std::cout << "ElapsedTime = " << sw.ElapsedTime() << " s" << std::endl;

    sw.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    sw.Stop();
    std::cout << "MicroElapsedTime = " << sw.MicroElapsedTime() << " μs" << std::endl;
    std::cout << "MilliElapsedTime = " << sw.MilliElapsedTime() << " ms" << std::endl;
    std::cout << "ElapsedTime = " << sw.ElapsedTime() << " s" << std::endl;

    //calc total desplay
    sw.LapStart();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    sw.LapStop();
    sw.LapStart();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    sw.LapStop();
    sw.LapStart();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    sw.LapStop();
    std::cout << "LAPMicroElapsedTime = " << sw.MicroLapTime() << " μs" << std::endl;
    std::cout << "LAPMilliElapsedTime = " << sw.MilliLapTime() << " ms" << std::endl;
    std::cout << "LAPElapsedTime = " << sw.LapTime() << " s" << std::endl;
    std::cout << "1 LAPMicroElapsedTime = " << sw.MicroLapTime("1") << " μs" << std::endl;
    std::cout << "1 LAPMilliElapsedTime = " << sw.MilliLapTime("1") << " ms" << std::endl;
    std::cout << "1 LAPElapsedTime = " << sw.LapTime("1") << " s" << std::endl;
    sw.LapStart("1");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    sw.LapStop();
    std::cout << "LAPMicroElapsedTime = " << sw.MicroLapTime() << " μs" << std::endl;
    std::cout << "LAPMilliElapsedTime = " << sw.MilliLapTime() << " ms" << std::endl;
    std::cout << "LAPElapsedTime = " << sw.LapTime() << " s" << std::endl;
    std::cout << "1 LAPMicroElapsedTime = " << sw.MicroLapTime("1") << " μs" << std::endl;
    std::cout << "1 LAPMilliElapsedTime = " << sw.MilliLapTime("1") << " ms" << std::endl;
    std::cout << "1 LAPElapsedTime = " << sw.LapTime("1") << " s" << std::endl;
    std::cout << "LAPMicroElapsedTime = " << sw.MicroTotalTime() << " μs" << std::endl;
    std::cout << "LAPMilliElapsedTime = " << sw.MilliTotalTime() << " ms" << std::endl;
    std::cout << "LAPElapsedTime = " << sw.TotalTime() << " s" << std::endl;
    sw.LapStart("fft");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    sw.LapStop();
}

void TestPthreadPool()
{
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
       //  pool.AddTask(ptcat::task::make_task([](ptcat::task::ISEXIT is_exit, std::string str)
       //  {
       //      while(true)
       //      {
       //          if (is_exit())
       //              break;
       //          std::this_thread::sleep_for(std::chrono::milliseconds(1));
       //          std::cout << str << std::endl;
       //      }
       //  }, "i miss you"));
       //
       //  pool.AddTask(ptcat::task::make_task([](ptcat::task::ISEXIT is_exit, int num, std::string str)
       // {
       //     while(true)
       //     {
       //         if (is_exit())
       //             break;
       //         std::this_thread::sleep_for(std::chrono::milliseconds(1));
       //         std::cout << num << "  " << str << std::endl;
       //     }
       // }, 100, "i miss you"));

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        num = pool.threads_run_count();
        std::cout << "------------- threads_run_count " << num << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));


        pool.AddTask(ptcat::task::make_task([](ptcat::task::ISEXIT is_exit, std::string str)
       {
            std::this_thread::sleep_for(std::chrono::seconds(5));
           std::cout << str << std::endl;
       }, "i miss you"));
        pool.AddTask(ptcat::task::make_task([](ptcat::task::ISEXIT is_exit, std::string str)
       {
            std::this_thread::sleep_for(std::chrono::seconds(1));
           std::cout << str << std::endl;
       }, "i miss you1"));

        pool.AddTask(ptcat::task::make_task([](ptcat::task::ISEXIT is_exit, std::string str)
       {
            std::this_thread::sleep_for(std::chrono::seconds(3));
           std::cout << str << std::endl;
       }, "i miss you2"));

        std::cout << "before wait" << std::endl;


         pool.WaitCurrentTask();
        std::cout << "after wait" << std::endl;

        pool.AddTask(ptcat::task::make_task([](ptcat::task::ISEXIT is_exit, std::string str)
      {
           std::this_thread::sleep_for(std::chrono::seconds(2));
          std::cout << str << std::endl;
      }, "i miss you111"));
        pool.AddTask(ptcat::task::make_task([](ptcat::task::ISEXIT is_exit, std::string str)
       {
            std::this_thread::sleep_for(std::chrono::seconds(1));
           std::cout << str << std::endl;
       }, "i miss you1222"));

         pool.AddTask(ptcat::task::make_task([](ptcat::task::ISEXIT is_exit, int num, std::string str)
        {
            while(true)
            {
                if (is_exit())
                    break;
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                std::cout << num << "  " << str << std::endl;
            }
        }, 100, "i miss you -------"));

        std::this_thread::sleep_for(std::chrono::seconds(5));
        pool.DestroyThreadPool();

        //
        // std::cout << "after wait" << std::endl;

        //pool.DestroyThreadPool();
        // while(true)
        // {
        //     std::this_thread::sleep_for(std::chrono::milliseconds(10));
        //     pool.AddTask(ptcat::task::make_task([](ptcat::task::ISEXIT is_exit, int num, std::string str)
        //     {
        //
        //            //std::this_thread::sleep_for(std::chrono::milliseconds(num));
        //            std::cout << num << "  " << str << std::endl;
        //     }, 10, "i test you,you just want to add it -------------------------------------"));
        //
        // }
        std::cout << "hello world " << std::endl;
    }
}

void TestPLogWriter()
{
    PLogWriter plog;
}