//
// Created by 27852 on 2026/5/6.
//

#include "ptcat/statistics/pstatistics.h"
#include "statistics/pdescriptive.hpp"

namespace ptcat {
    namespace palgo {//算法命名空间
        //中位数获取
        float GetMedianElement(ptcat::Cat1D<float> values){
            float* ptr = values.Data();
            int count = values.Cols();
            return statistics::descriptive::GetMedianElement(ptr, count);
        }

        //中位数获取
        double GetMedianElement(ptcat::Cat1D<double> values){
            double* ptr = values.Data();
            int count = values.Cols();
            return statistics::descriptive::GetMedianElement(ptr, count);
        }

        //加权平均获取峰值索引
        void GetPeakIndexByWAvg(ptcat::Cat1D<float> input, int weight, float& peak_index){
            float* ptr = input.Data();
            int count = input.Cols();
            statistics::descriptive::GetPeakIndexByWAvg(ptr, count, weight, peak_index);
        }

        //加权平均获取峰值索引
        void GetPeakIndexByWAvg(ptcat::Cat1D<double> input, int weight, double& peak_index){
            double* ptr = input.Data();
            int count = input.Cols();
            statistics::descriptive::GetPeakIndexByWAvg(ptr, count, weight, peak_index);
        }
    }
}
