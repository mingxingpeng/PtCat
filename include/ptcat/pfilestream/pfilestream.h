//
// Created by 27852 on 2025/8/27.
//

#ifndef PFILESTREAM_H
#define PFILESTREAM_H

#include <filesystem>
#include <string>

namespace ptcat {
    namespace pfilestream {
        ///读写方式
        enum class RWMode {
            LINEBYLINE = 0,//逐行读写
            BATCH = 1,
        };

        ///错误码
        enum class ErrCode {
            FILE_DOES_NOT_EXIST = -1,//文件不存在
            FILE_CANNOT_BE_ACCESSED = -2,//文件无法访问
            INSUFFICIENT_PERMISSION = -3,//权限不足
            INSUFFICIENT_DISK_SPACE = -4,//磁盘空间不足
            THE_READING_AND_WRITING_PROCESS_IS_INTERRUPTED = -5,//读写过程中断
        };

        ///打开模式
        enum class OpenMode{
            READONLY = 0,//只读
            WRITEONLY = 1,//只写
            READWRITE = 2,//读写
            APPEND = 3,//添加
            CREATE = 4,//创建
        };

        ///缓冲策略
        enum class BufferStrategy {
            DEFAULT = 0,//系统默认
            LINEBUFFER = 1,//行缓冲
            FULLBUFFER = 2,//全缓冲
            NOBUFFER = 3,//无缓冲
        };

        ///文件处理类
        class PFileStream {
        public :
            //防止出现类似与字符串的那种隐士转换
            explicit PFileStream() = default;
            explicit PFileStream(std::string path, OpenMode mode = OpenMode::READWRITE, BufferStrategy bufferStrategy = BufferStrategy::DEFAULT);
            ~PFileStream();

            PFileStream(const PFileStream& p) = delete;
            PFileStream(const PFileStream&& p) = delete;
            PFileStream& operator=(const PFileStream& p) = delete;
            PFileStream& operator=(const PFileStream&& p) = delete;
        };
    }
}
#endif //PFILESTREAM_H
