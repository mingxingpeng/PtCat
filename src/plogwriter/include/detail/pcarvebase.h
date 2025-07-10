//
// Created by Administrator on 2025/3/18.
//

#ifndef PCARVEBASE_H
#define PCARVEBASE_H

#include "pcarve.h"
/*
Skin 为输出目标
 */
namespace ptcat{
    namespace plog{
        class PCarveBase : public PCarve{
            //写日志
            virtual void Log() = 0;
            //刷新缓存
            virtual void Flush() = 0;

            //设置日志输出格式，暂时先不动
            virtual void SetPattern() = 0;



        };
    }
}

#endif //PCARVEBASE_H
