#include <iostream>
#include "ptcat/pthreadpool/pthreadpool.h"
#include "ptcat/plogwriter/plogwirter.h"
#include <functional>
#include "ptcat/pclock/pstopwatch.h"
#include "ptcat/pclock/pchronograph.h"
#include "ptcat/pfilestream/pfolder.h"
#include "ptcat/pfilestream/pfile.h"
#include "ptcat/pfilestream/ptextfilestream.h"

void TestClock();
void TestPthreadPool();
void TestPLogWriter();
void TestFile();



int main()
{
    // TestClock();
    // TestPLogWriter();
    TestPthreadPool();
    // TestFile();

    return 0;
}

void TestFile() {
    // ptcat::pfilestream::PFolder folder("E:\\pmx\\1213");
    // bool is_exists = folder.IsExists();
    // bool is_succ = folder.Create();
    // bool is_succ1 = folder.IsExistsAndCreate();
    // while (true) {
    //     std::cout << folder.Size() << std::endl;
    //     std::this_thread::sleep_for(std::chrono::seconds(1));
    // }
    // bool is_suuc1 = folder.Delete();
    // bool issuc = folder.Rename("pm121231231x1212");
    // folder.Move("E:\\test\\");

    // ptcat::pfilestream::PFile pfile;
    // pfile.Open("C:\\Users\\27852\\Desktop\\img\\1\\2\\1000.jpg");
    // pfile.Rename("test.jpg");
    // pfile.Delete();
    // pfile.Copy("C:\\Users\\27852\\Desktop\\1");
    // pfile.Move("C:\\Users\\27852\\Desktop\\1");
    // pfile.Create();

    ptcat::pfilestream::PTextFileStream stream;
    stream.Open("E:\\test\\img\\12\\2\\dsa\\0.txt", ptcat::pfilestream::OpenMode::W_APPEND);
    int size = stream.Size();
    std::string line = "hello world";
    stream.WriteLine("zhenshi", true);
    stream.WriteLine("1221312312312313123123213");
    stream.WriteAll("11111111111111111111111111111111111111111111111111111111");
    stream.WriteAll("11111111111111111111111111111111111111111111111111111111");
    stream.WriteAll("11111111111111111111111111111111111111111111111111111111");
    stream.WriteAll("11111111111111111111111111111111111111111111111111111111");
    stream.WriteAll("11111111111111111111111111111111111111111111111111111111");
    stream.Flush();
    stream.ReadLine(line);
    std::cout << line << std::endl;
    stream.ReadLine(line);
    std::cout << line << std::endl;
    stream.ReadLine(line);
    std::cout << line << std::endl;
    std::cout << "----------------------------------" << std::endl;
    stream.ReadAll(line);
    std::cout << line << std::endl;
    stream.SeekPos(0, ptcat::pfilestream::Pos::BEGIN);
    // size = stream.Size();
    stream.SeekPos(0, ptcat::pfilestream::Pos::END);

    stream.Close();
}

void TestClock(){
    ptcat::clock::StopWatch sw;
    sw.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    sw.Stop();
    std::cout << "MicroElapsedTime = " << sw.MicroElapsedTime() << " ?s" << std::endl;
    std::cout << "MilliElapsedTime = " << sw.MilliElapsedTime() << " ms" << std::endl;
    std::cout << "ElapsedTime = " << sw.ElapsedTime() << " s" << std::endl;

    sw.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    sw.Stop();
    std::cout << "MicroElapsedTime = " << sw.MicroElapsedTime() << " ?s" << std::endl;
    std::cout << "MilliElapsedTime = " << sw.MilliElapsedTime() << " ms" << std::endl;
    std::cout << "ElapsedTime = " << sw.ElapsedTime() << " s" << std::endl;

    //calc total desplay
    sw.LapStart("1");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    sw.LapStop("1");
    sw.LapStart("1");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    sw.LapStop("1");
    sw.LapStart("1");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    sw.LapStop("1");
    std::cout << "LAPMicroElapsedTime = " << sw.MicroLapTime("1") << " ?s" << std::endl;
    std::cout << "LAPMilliElapsedTime = " << sw.MilliLapTime("1") << " ms" << std::endl;
    std::cout << "LAPElapsedTime = " << sw.LapTime("1") << " s" << std::endl;
    std::cout << "2 LAPMicroElapsedTime = " << sw.MicroLapTime("2") << " ?s" << std::endl;
    std::cout << "2 LAPMilliElapsedTime = " << sw.MilliLapTime("2") << " ms" << std::endl;
    std::cout << "2 LAPElapsedTime = " << sw.LapTime("2") << " s" << std::endl;
    sw.LapStart("2");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    sw.LapStop("2");
    std::cout << "LAPMicroElapsedTime = " << sw.MicroLapTime("1") << " ?s" << std::endl;
    std::cout << "LAPMilliElapsedTime = " << sw.MilliLapTime("1") << " ms" << std::endl;
    std::cout << "LAPElapsedTime = " << sw.LapTime("1") << " s" << std::endl;
    std::cout << "2 LAPMicroElapsedTime = " << sw.MicroLapTime("2") << " ?s" << std::endl;
    std::cout << "2 LAPMilliElapsedTime = " << sw.MilliLapTime("2") << " ms" << std::endl;
    std::cout << "2 LAPElapsedTime = " << sw.LapTime("2") << " s" << std::endl;

    sw.LapStart("3");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    sw.LapStart("4");
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    sw.LapStop("4");
    sw.LapStop("3");
    std::cout << "3 LAPMicroElapsedTime = " << sw.MicroLapTime("3") << " ?s" << std::endl;
    std::cout << "3 LAPMilliElapsedTime = " << sw.MilliLapTime("3") << " ms" << std::endl;
    std::cout << "3 LAPElapsedTime = " << sw.LapTime("3") << " s" << std::endl;
    std::cout << "4 LAPMicroElapsedTime = " << sw.MicroLapTime("4") << " ?s" << std::endl;
    std::cout << "4 LAPMilliElapsedTime = " << sw.MilliLapTime("4") << " ms" << std::endl;
    std::cout << "4 LAPElapsedTime = " << sw.LapTime("4") << " s" << std::endl;
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

        pool.AddTask(ptcat::task::make_task([&](ptcat::task::ISEXIT is_exit)
      {
            auto task = ptcat::task::make_task([](ptcat::task::ISEXIT is_exit, std::string str)
        {
             // std::this_thread::sleep_for(std::chrono::seconds(3));
            std::cout << str << std::endl;
        }, "i miss you");
         task->SetWaitStatus(false);
         pool.AddTask(task);

         auto task1 = ptcat::task::make_task([](ptcat::task::ISEXIT is_exit, std::string str)
        {
             // std::this_thread::sleep_for(std::chrono::seconds(3));
            std::cout << str << std::endl;
        }, "i miss you1");
         task1->SetWaitStatus(false);
         pool.AddTask(task1);

         auto task2 = ptcat::task::make_task([](ptcat::task::ISEXIT is_exit, std::string str)
        {
             std::this_thread::sleep_for(std::chrono::seconds(3));
            std::cout << str << std::endl;
        }, "i miss you1_test");
         task2->SetWaitStatus(false);
         pool.AddTask(task2);

         std::cout << "before wait" << std::endl;
          pool.WaitCurrentTask();
         std::cout << "after wait" << std::endl;
      }));


        std::this_thread::sleep_for(std::chrono::seconds(30));

        pool.AddTask(ptcat::task::make_task([](ptcat::task::ISEXIT is_exit, std::string str)
       {
            std::this_thread::sleep_for(std::chrono::seconds(3));
           std::cout << str << std::endl;
       }, "i miss you2"));


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

        //  pool.AddTask(ptcat::task::make_task([](ptcat::task::ISEXIT is_exit, int num, std::string str)
        // {
        //     while(true)
        //     {
        //         if (is_exit())
        //             break;
        //         std::this_thread::sleep_for(std::chrono::milliseconds(1));
        //         std::cout << num << "  " << str << std::endl;
        //     }
        // }, 100, "i miss you -------"));

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
    ptcat::plog::PLogWriter plog;
}