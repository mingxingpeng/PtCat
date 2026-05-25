//
// Created by 27852 on 2026/5/25.
//

#ifndef PTCAT_PFSTREAM_H
#define PTCAT_PFSTREAM_H

#include "ptcat/common/common.h"
#include <fstream>
#include <cassert>

namespace ptcat {
    namespace fstream {

        //目前只设置只读只写两种模式
        enum FStreamMode {
            READONLY = 0,//只读
            WRITEONLY = 1,//只写
        };

        class DLL_API PFStream {

        protected:
            std::fstream fstream_;//文件流

            FStreamMode stream_mode_;//文件流模式

            bool IsNeedMode(FStreamMode mode);//判断是否是当前需要的模式
        public:
            PFStream() = default;

            ~PFStream();

            virtual bool Open(std::string file_path, FStreamMode mode);

            virtual bool IsOpen();

            virtual void Close();

            template<class T>
            PFStream& operator<<(const T& data){
                assert(IsNeedMode(FStreamMode::WRITEONLY));
                fstream_ << data;
                return *this;
            }

            template<class T>
            PFStream& operator>>(T& data){
                assert(IsNeedMode(FStreamMode::READONLY));
                fstream_ >> data;
                return *this;
            }
        };

    }
}
#endif //PTCAT_PFSTREAM_H
