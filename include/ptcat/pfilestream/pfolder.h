//
// Created by 27852 on 2025/8/27.
//

#ifndef PFOLDER_H
#define PFOLDER_H
#include "pio.h"
#include "vector"
#include "ptcat/common/common.h"

namespace ptcat {
    namespace pfilestream {
        ///文件夹操作
        class DLL_API PFolder : public PIO{
        public:
            explicit PFolder(std::string folder);
            ~PFolder();

            PFolder(const PFolder& p) = delete;
            PFolder(const PFolder&& p) = delete;
            PFolder& operator=(const PFolder& p) = delete;
            PFolder& operator=(const PFolder&& p) = delete;

            bool IsExists() const override;//判断文件夹是否存在

            bool IsExistsAndCreate();//判断文件夹是否存在，不存在就创建

            bool IsDirectory(const std::string path);//用于判断是否是文件夹

            uint64_t Size() override;//文件夹中文件数量

            bool Rename(std::string new_name) override;//重命名文件夹

            bool Delete() override;//删除文件夹

            bool Create() override;//创建文件夹

            bool Move(std::string new_path) override;//移动文件夹

            bool Copy(std::string new_pos) override;//创建副本

            void GetAllFromFolder(std::vector<Path>& file_paths, std::string extension = "");//获取目录下所有的文件路径,根据后缀可筛选
        };
    }
}


#endif //PFOLDER_H
