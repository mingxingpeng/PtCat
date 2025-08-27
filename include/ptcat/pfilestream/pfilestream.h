//
// Created by 27852 on 2025/8/27.
//

#ifndef PFILESTREAM_H
#define PFILESTREAM_H


#include "pio.h"

namespace ptcat {
    namespace pfilestream {
        ///文件处理类
        // class PFileStream  : public PIO {
        // private:
        //     OpenMode mode_;//打开方式
        //     BufferStrategy bufferStrategy_;//缓冲模式
        //
        // public :
        //     //防止出现类似与字符串的那种隐士转换
        //     explicit PFileStream() = default;
        //     explicit PFileStream(const std::string& path, OpenMode mode = OpenMode::READWRITE, BufferStrategy bufferStrategy = BufferStrategy::DEFAULT);
        //     virtual ~PFileStream();
        //
        //     PFileStream(const PFileStream& p) = delete;
        //     PFileStream(const PFileStream&& p) = delete;
        //     PFileStream& operator=(const PFileStream& p) = delete;
        //     PFileStream& operator=(const PFileStream&& p) = delete;
        //
        //     //打开方式
        //     bool Open(const std::string& path, OpenMode mode = OpenMode::READWRITE, BufferStrategy bufferStrategy = BufferStrategy::DEFAULT);
        //
        //     bool IsExists() const override;//判断文件是否存在
        //
        //     uint64_t Size() const override;//尺寸
        //
        //     bool IsOpen() const;//文件是否打开、
        //
        //     void Rename(std::string new_name) override;//重命名文件
        //
        //     void Delete() override;//删除文件
        //
        //     void Create() override;//创建文件
        //
        //     void Move(std::string new_path) override;//移动文件
        //
        //     void Copy(std::string new_pos) override;//创建副本
        //
        //     void Close();//关闭文件
        //
        //     void Flush();//刷新
        //
        //
        //     void ReadAll(std::string& content);//读取全部内容
        //
        //     void ReadLine(std::string& line);//读取一行内容
        //
        //     void WriteAll(const std::string& content);//写入当前全部内容
        //
        //     void WirteLine(const std::string& line);//写入一行内容
        //
        //     void ReadBulk(char* buf, size_t buf_size);//批量读取
        //
        //     void WriteBulk(const char* buf, size_t buf_size);//批量写入
        // };
    }
}
#endif //PFILESTREAM_H
