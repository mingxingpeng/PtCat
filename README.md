# PtCat

## (一) PThreadPool 线程池库

### PThreadPool：线程池类

#### ① 创建线程池
    
    void CreateThreadPool(const int& threads_size);

#### ② 销毁线程池

    void DestroyThreadPool();

#### ③ 添加线程

    void AddThreads(const int&  thread_size);

#### ④ 添加任务

    void AddTask(const std::shared_ptr<Task>& task);

#### ④ 获取正在运行线程的数量

    int threads_run_count()

#### ④ 判断当前线程是否退出

    is_exit()



### Task: Task 基类

### PTask：模板类，继承于 Task，任务类，用于创建任务传递给线程池

#### 构造函数

    PTask(Function&& func, Args&&... args) : func_(std::forward<Function>(func)), args_(std::make_tuple(std::forward<Args>(args)...))
    {

    }

①创建任务对象的时候将对应的函数以及函数对应的参数传递进来

②构造函数第一个参数被强制要求为 std::function<bool()> 类型，用于联动 PThreadPool 线程池中 is_exit() 函数，如果该任务是一个死循环任务，可以帮助设置死循环任务停止

③任务类帮助函数

    template<typename Function, typename... Args>
    auto make_task(Function&& func, Args&&... args)
   用于构建任务类共享智能指针

       auto task = ptcat::task::make_task([](ptcat::task::ISEXIT is_exit, std::string str)
       {
           while(true)
           {
                if (is_exit())//如果是死循环函数必须加上，当线程池推出时，该函数帮助推出死循环
                    break;
                ...
           }
       }, "i miss you");
### 示例：

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
