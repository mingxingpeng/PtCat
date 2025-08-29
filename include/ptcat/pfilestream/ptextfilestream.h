//
// Created by 27852 on 2025/8/27.
//

#ifndef PFILESTREAM_H
#define PFILESTREAM_H

/*
pmx: 输出会有乱码问题，我这里解决是将 CLion 设置里面设置了默认输出模式是 GB2312,没有使用 UTF-8,后续需要再说吧
 */
#include "pfile.h"
#include <mutex>
#include <shared_mutex>
#include <fstream>

namespace ptcat {
    namespace pfilestream {

        //读写模式
        enum class ReadWriteMode : int{
            READONLY,
            WRITEONLY,
            DOUBLE
        };


        class DLL_API PTextFileStream  : public PFile {
        private:
            OpenMode mode_;//模式
            BufferStrategy bufferStrategy_;//缓存策略
            std::shared_mutex shared_mtx_;//锁
            std::ifstream ifstream_;//输入流
            std::ofstream ofstream_;//输出流
            ReadWriteMode rw_mode_;

            void SyncReadStream();
        public:
            //构造器
            explicit PTextFileStream();
            explicit PTextFileStream(const std::string& path, OpenMode mode = OpenMode::RW_APPEND, BufferStrategy bufferStrategy = BufferStrategy::DEFAULT);
            virtual ~PTextFileStream();

            PTextFileStream(const PTextFileStream& p) = delete;
            PTextFileStream(const PTextFileStream&& p) = delete;
            PTextFileStream& operator=(const PTextFileStream& p) = delete;
            PTextFileStream& operator=(const PTextFileStream&& p) = delete;

            //????
            StatusCode Open(const std::string& path, OpenMode mode = OpenMode::RW_APPEND, BufferStrategy bufferStrategy = BufferStrategy::DEFAULT);

            uint64_t Size() override;//???????

            bool IsOpen();//??????

            void Close();//????

            StatusCode Clear();//??????

            /*
            刷新缓冲区，将数据立即写入文件（默认情况下缓冲区满时自动刷新）
             */
            StatusCode Flush();//???????????????????????

            StatusCode SeekPos(uint64_t offest, Pos pos, ReadWriteMode pos_mode = ReadWriteMode::DOUBLE);

            StatusCode ReadAll(std::string& content);//??????

            StatusCode ReadLine(std::string& line);//??????

            StatusCode WriteAll(const std::string& content);//????????

            StatusCode WriteLine(const std::string& line, bool add_new_line = true);//??????

            void Test();
        };
    }
}
#endif //PFILESTREAM_H
