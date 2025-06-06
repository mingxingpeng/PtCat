//
// Created by Administrator on 2025/3/12.
//

/*

 */

#ifndef PLOGWIRTER_H
#define PLOGWIRTER_H

namespace ptcat{
    namespace carve{
        enum class LogLevel{
            TARCE = 0,//杂项
            DEBUG,//测试日志
            WARN,//警告
            INFO,//信息
            ERROR,//错误信息
        };

        class PCarve
        {
        public:
            PCarve();

            //写日志
            virtual void Log() = 0;
            //刷新缓存
            virtual void Flush() = 0;

            //设置日志格式，暂时先不设置
            //virtual void SetPattern() = 0;

            //设置最小输出日志等级
            void SetMinLevel(const LogLevel& log_level){
                min_level_ = log_level;
            }

            //设置是否应该记录某一个日志级别,比如日志级别分为 1 2 3 4 5 6，我这最低日志等级为 2，则 对于 2 3 4 5 6 这些级别的日志都会被输出，而 1 不会被输出
            bool ShouldLog(const LogLevel& log_level) const{
                if(log_level < min_level_){
                    return false;
                }
                return true;
            }

        private:
            LogLevel min_level_;
        };
    }
}
#endif //PLOGWIRTER_H
