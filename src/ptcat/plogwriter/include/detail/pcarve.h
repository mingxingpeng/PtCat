//
// Created by Administrator on 2025/3/12.
//

/*
日志级别 (Log Levels)
支持多种日志级别：TRACE, DEBUG, INFO, WARN, ERROR, FATAL
允许运行时动态调整日志级别
 */

#ifndef PCARVE_H
#define PCARVE_H

#include "core/core.h"

namespace ptcat{
    namespace plog{
        /*
        设置日志的输出模式以及等级
         */
        class PCarve
        {
        private:
            LogLevel min_level_;
            WriteMethod write_method_;

        public:
            PCarve();

            //写日志
            virtual void Log() = 0;
            //刷新缓存
            virtual void Flush() = 0;

            //设置日志输出格式，暂时先不动
            virtual void SetPattern() = 0;

            //设置最小输出日志等级，该方法可以用于支持在工作过程中进行替换等级
            void SetMinLevel(const LogLevel& log_level){
                min_level_ = log_level;
            }

            //设置写入方式
            void SetWriteMethod(const WriteMethod& write_method){
                write_method_ = write_method;
            }

            //获取写入方式
            const WriteMethod GetWriteMethod( ) const{
                return write_method_;
            }

            //设置是否应该记录某一个日志级别,比如日志级别分为 1 2 3 4 5 6，我这最低日志等级为 2，则 对于 2 3 4 5 6 这些级别的日志都会被输出，而 1 不会被输出
            bool ShouldLog(const LogLevel& log_level) const{
                if(log_level < min_level_){
                    return false;
                }
                return true;
            }
        };
    }
}
#endif //PCARVE_H
