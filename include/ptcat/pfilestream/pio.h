//
// Created by 27852 on 2025/8/27.
//

#ifndef IO_H
#define IO_H

#include <filesystem>
#include <string>

namespace ptcat {
    namespace pfilestream {
        ///读写方式
        enum class RWMode : int {
            LINEBYLINE = 0,//逐行读写
            BATCH = 1,
        };

        ///错误码
        enum class ErrCode : int {
            FILE_DOES_NOT_EXIST = -1,//文件不存在
            FILE_CANNOT_BE_ACCESSED = -2,//文件无法访问
            INSUFFICIENT_PERMISSION = -3,//权限不足
            INSUFFICIENT_DISK_SPACE = -4,//磁盘空间不足
            THE_READING_AND_WRITING_PROCESS_IS_INTERRUPTED = -5,//读写过程中断
        };

        ///打开模式
        enum class OpenMode : int {
            READONLY = 0,//只读
            WRITEONLY = 1,//只写
            READWRITE = 2,//读写
            APPEND = 3,//添加
            CREATE = 4,//创建
        };

        ///缓冲策略
        enum class BufferStrategy : int {
            DEFAULT = 0,//系统默认
            LINEBUFFER = 1,//行缓冲
            FULLBUFFER = 2,//全缓冲
            NOBUFFER = 3,//无缓冲
        };


        ///文件结构
        ///如果是文件夹的话，后缀为空
        struct Path{
            std::string name;//名称
            std::string extension;//文件后缀
            std::string full_path;//全路径

            Path(std::string name, std::string extension, std::string full_path) : name(name), extension(extension), full_path(full_path) {}
        };

        namespace fs = std::filesystem ;

        class PIO {
        protected:
            bool is_exist_;//是否存在
            uint64_t size_;//数量
            fs::path path_;
        public:
            explicit PIO() = default;
            virtual ~PIO() = default;

            PIO(const PIO& p) = delete;
            PIO(const PIO&& p) = delete;
            PIO& operator=(const PIO& p) = delete;
            PIO& operator=(const PIO&& p) = delete;

            virtual bool IsExists() const = 0;//判断文件是否存在

            virtual  uint64_t Size() = 0;//尺寸

            virtual  bool Rename(std::string new_name) = 0;//重命名文件

            virtual  bool Delete() = 0;//删除文件

            virtual  bool Create() = 0;//创建文件

            virtual  bool Move(std::string new_pos) = 0;//移动文件

            virtual bool Copy(std::string new_pos) = 0;//创建副本
        };
    }
}
#endif //IO_H
