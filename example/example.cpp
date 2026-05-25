#include <ptcat/common/cat.hpp>
#include <iostream>
#include "ptcat/utils/clock/pstopwatch.h"
#include "ptcat/utils/fstream/ptextstream.h"
#include "ptcat/utils/string/pstring.h"


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

//    double sigma = 1;
//    auto cat = ptcat::palgo::GetLogKernel(sigma);
//    auto cat1 = ptcat::palgo::GetLogKernel(sigma);
//    auto cat2 = ptcat::palgo::GetLogKernel(sigma);
//    auto cat3 = ptcat::palgo::GetLogKernel(sigma);

//    ptcat::clock::StopWatch sw;
//    sw.Start();
//    ptcat::palgo::fft::PFFT fft;
//    ptcat::palgo::fft::Range range(30, 40);
//    fft.PFFTInit(1000, range, true);
//    fft.PFFTDeInit();
//    fft.PFFTInit(1000, range);
////    ptcat::palgo::fft::Range range1(10, 40);
////    fft.PFFTInit(1000, range1);
//    sw.Stop();
//    std::cout << "time: " << sw.MilliElapsedTime() << std::endl;

//    ptcat::fstream::PTextStream pts;
//    bool is_open = pts.Open("C:\\Users\\27852\\Desktop\\test.csv", ptcat::fstream::FStreamMode::READONLY);
//    if (is_open){
//        std::string line1;
////        pts.WriteLineSync(line1);
//        pts.ReadLineSync(line1);
//        pts.ReadLineSync(line1);
//        pts.ReadLineSync(line1);
//        pts.ReadLineSync(line1);
//        pts.ReadLineSync(line1);
//        pts.ReadLineSync(line1);
////        pts.ReadLineSync(line1);
////        pts.ReadLineSync(line1);
////        pts.ReadLineSync(line1);
////        pts.ReadLineSync(line1);
////        pts >> line1;
////        pts >> line1;
////        pts >> line1;
////        pts >> line1;
////        pts >> line1;
////        std::string line2;
////        pts >> line1 >> line2;
//        std::vector<std::string> res;
//        ptcat::string::Split(line1, ',', res);
//
//    }
//
//    ptcat::fstream::PTextStream pts1;
//    bool is_open1 = pts1.Open("C:\\Users\\27852\\Desktop\\test1.csv", ptcat::fstream::FStreamMode::WRITEONLY);
//    if (is_open1){
//        std::string line1 = "nihao,woshi,shuaibi";
//        bool is_succ = pts1.WriteLineSync(line1);
//        pts1 << line1 << "\n";
//        pts1 << line1 << "\n";
//        pts1 << line1 << "\n";
//        pts1 << line1 << "\n";
//        pts1 << line1 << "\n";
//
////        pts1.ReadLineSync(line1);
//    }

    std::string str1 = "ABSDswqqA";
    std::string str2 = "ABSDswqqA";
    ptcat::string::ToLower(str1);
    ptcat::string::ToUpper(str2);

    std::string res;
    ptcat::string::ToLower(str2, res);
    ptcat::string::ToUpper(str1, res);

    std::string str3 = " 1 2 3 4 5 6 ";
    ptcat::string::Trim(str3);
    std::string str4 = " A 2 3 4 5 b ";
    ptcat::string::Trim(str3, res);
    std::string str5 = " A 2 3 4 5 b ";
    ptcat::string::TrimBegin(str5);
    std::string str6 = " A 2 3 4 5 b ";
    ptcat::string::TrimBegin(str6, res);
    std::string str7 = " A 2 3 4 5 b ";
    ptcat::string::TrimEnd(str7);
    std::string str8 = " A 2 3 4 5 b ";
    ptcat::string::TrimEnd(str8, res);

    bool is_succ = ptcat::string::Contains("12345", "23");
    is_succ = ptcat::string::Contains("12345", "54");

    std::string path = "D://cas??//212jd.csv";
    is_succ = ptcat::string::StartsWith(path, "D:");
    is_succ = ptcat::string::StartsWith(path, "E:");
    is_succ = ptcat::string::EndsWith(path, ".csv");
    is_succ = ptcat::string::EndsWith(path, ".jpg");
    return 0;
}