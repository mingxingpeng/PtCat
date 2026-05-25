//
// Created by 27852 on 2026/5/25.
//

#include "ptcat/utils/fstream/ptextstream.h"
#include <string>

namespace ptcat {
    namespace fstream {

        //写入单行
        bool PTextStream::WriteLineSync(const std::string& text){
            assert(IsNeedMode(FStreamMode::WRITEONLY));
            if (!IsOpen()) return false;
            fstream_.write(text.c_str(), text.length());
            //写入换行符
            fstream_.write("\n", 1);
            return !fstream_.fail();//判断有没有成功
        }

        //读取单行
        bool PTextStream::ReadLineSync(std::string& text){
            assert(IsNeedMode(FStreamMode::READONLY));
            if (!IsOpen()) return false;
            text.clear();
            return static_cast<bool>(std::getline(fstream_, text));
        }

    }
}