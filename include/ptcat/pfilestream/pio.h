//
// Created by 27852 on 2025/8/27.
//

#ifndef IO_H
#define IO_H

#include <filesystem>
#include <string>
#include "ptcat/common/common.h"

namespace ptcat {
    namespace pfilestream {
        ///读写方式
        enum class RWMode : int {
            LINEBYLINE = 0,//逐行读写
            BATCH = 1,
        };

        ///错误码
        enum class StatusCode : int {
            SUCCESS = 1,
            FILE_EOF = 0,
            FILE_DOES_NOT_EXIST = -1,//文件不存在
            FILE_CANNOT_BE_ACCESSED = -2,//文件无法访问
            INSUFFICIENT_PERMISSION = -3,//权限不足
            INSUFFICIENT_DISK_SPACE = -4,//磁盘空间不足
            RW_PROCESS_IS_INTERRUPTED = -5,//读写过程中断
            FILE_NOT_OPEN = -6,
            NOT_FILE = -7,
            CURR_STREAM_IS_OCCUPIED = -8,//当前流被占用
            FAILED = -9,//错误
        };

        ///打开模式
        enum class OpenMode : int {
            READONLY = 0,//只读， 仅打开文件用于读取，不允许写入
            WRITEONLY = 1,//只写，打开文件用于写入，若文件已存在则清空原有内容；若不存在则创建新文件。
            READWRITE = 2,//读写，同时具备读写权限，打开时不改变原有内容；要求文件必须已存在
            W_APPEND= 3,//追加内容，打开文件用于写入，新内容强制追加到文件末尾；若文件不存在则创建。
            RW_APPEND = 4,//读写 + 添加，可读取文件内容，但写入时只能追加到末尾；要求文件必须已存在。
            RW_TRUNC = 5,//读写 + 清空文件，先清空文件原有内容，再以读写模式打开；若文件不存在则创建。
        };

        ///缓冲策略
        enum class BufferStrategy : int {
            DEFAULT = 0,//系统默认
            LINEBUFFER = 1,//行缓冲
            FULLBUFFER = 2,//全缓冲
            NOBUFFER = 3,//无缓冲
        };


        enum class Pos : uint64_t {
            BEGIN = 0,
            END = -1,
        };


        ///文件结构
        ///如果是文件夹的话，后缀为空
        struct Path{
            std::string name;//名称
            std::string extension;//文件后缀
            std::string full_path;//全路径

            Path(std::string name, std::string extension, std::string full_path) : name(name), extension(extension), full_path(full_path) {}
        };

        namespace fs = std::filesystem;

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

            virtual  bool Rename(const std::string& new_name) = 0;//重命名文件

            virtual  bool Delete() = 0;//删除文件

            virtual  bool Create() = 0;//创建文件

            virtual  bool Move(const std::string& new_pos) = 0;//移动文件
        };
    }
}
#endif //IO_H
