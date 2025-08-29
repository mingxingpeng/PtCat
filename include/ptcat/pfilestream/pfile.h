//
// Created by 27852 on 2025/8/27.
//

#ifndef PFILE_H
#define PFILE_H

#include "pio.h"
#include "fstream"

namespace ptcat {
    namespace pfilestream {
        class DLL_API PFile : public PIO {
        public:
            explicit PFile();
            explicit PFile(const std::string& path);
            virtual ~PFile();

            PFile(const PFile& p) = delete;
            PFile(const PFile&& p) = delete;
            PFile& operator=(const PFile& p) = delete;
            PFile& operator=(const PFile&& p) = delete;

            bool View(const std::string& path);//查看文件

            bool IsFile();//判断是否是文件

            virtual bool IsExists() const override;//判断文件是否存在

            virtual  uint64_t Size() override;

            virtual bool Rename(const std::string& new_name) override;//重命名文件

            virtual bool Delete() override;//删除文件

            virtual bool Create() override;//创建文件

            virtual bool Move(const std::string& new_path) override;//移动文件

            virtual bool Copy(const std::string& new_pos);//创建副本
        };
    }
}

#endif //PFILE_H
