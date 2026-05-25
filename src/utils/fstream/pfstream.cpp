//
// Created by 27852 on 2026/5/25.
//

#include "ptcat/utils/fstream/pfstream.h"

namespace ptcat {
    namespace fstream {
        PFStream::~PFStream(){
            if(IsOpen())
                Close();
        }

        bool PFStream::IsNeedMode(FStreamMode mode){
            return stream_mode_ == mode;
        }

        bool PFStream::Open(std::string file_path, FStreamMode mode){
            stream_mode_ = mode;
            switch (stream_mode_) {
                case FStreamMode::READONLY:
                    fstream_.open(file_path, std::ios::in);
                    break;
                case FStreamMode::WRITEONLY:
                    fstream_.open(file_path, std::ios::out);
                    break;
            }
            //这里直接返回是否打开了
            return IsOpen();
        }

        bool PFStream::IsOpen(){
            return fstream_.is_open();
        }

        void PFStream::Close(){
            fstream_.close();
        }
    }
}
