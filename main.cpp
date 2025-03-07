#include <iostream>
#include <tuple>
#include "pthreadpool.h"
#include <functional>
#include <list>


void printSum(int a, int b)
{
    std::cout << "pengmingxingzhenshuaia,wocao " << a  << std::endl;
}

void printSum1(int a, std::string b)
{
    std::cout << "pengmingxingzhenshuaia,wocao " << b  << std::endl;
}

int main()
{
    ptcat::PTask<std::function<void(int, int)>, int, int> t(printSum, 10, 20);
    t.Run();
    std::list<std::shared_ptr<ptcat::Task>> tt;
    tt.push_back(std::make_shared<ptcat::PTask<std::function<void(int, int)>, int, int>>(printSum, 50, 20));
    tt.push_back(std::make_shared<ptcat::PTask<std::function<void(int, std::string)>, int, std::string>>(printSum1, 10, "wonima"));
    tt.front()->Run();
    tt.back()->Run();



    return 0;
}
