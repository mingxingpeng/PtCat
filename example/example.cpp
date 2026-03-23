#include <ptcat/common/cat.hpp>
#include <iostream>
#include "ptcat/filter/pfilter.h"

int main(){
//    ptcat::Cat<double> cat(10, 10);
//    ptcat::Cat<double> cat1(20, 20);
//    cat1 = cat;
//    ptcat::Cat<double> cat2(cat);

//    double* img = new double[100];
//    std::cout << img << std::endl;
//    ptcat::Cat<double> cat3(10, 10, img);
//    std::cout << cat3.Data() << std::endl;
//    ptcat::Cat<double> cat4(cat3);
//    std::cout << cat4.Data() << std::endl;
//    delete [] img;
//    img = nullptr;

//    ptcat::Cat<double> cat = ptcat::Cat<double>(10, 10);

    double sigma = 1;
    auto cat = ptcat::palgo::GetLogKernel(sigma);
    auto cat1 = ptcat::palgo::GetLogKernel(sigma);
    auto cat2 = ptcat::palgo::GetLogKernel(sigma);
    auto cat3 = ptcat::palgo::GetLogKernel(sigma);
    return 0;
}