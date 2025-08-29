//
// Created by 27852 on 2025/8/27.
//

#ifndef PFILESTREAM_H
#define PFILESTREAM_H

/*
pmx: ��������������⣬���������ǽ� CLion ��������������Ĭ�����ģʽ�� GB2312,û��ʹ�� UTF-8,������Ҫ��˵��
 */
#include "pfile.h"
#include <mutex>
#include <shared_mutex>
#include <fstream>

namespace ptcat {
    namespace pfilestream {

        //��дģʽ
        enum class ReadWriteMode : int{
            READONLY,
            WRITEONLY,
            DOUBLE
        };


        class DLL_API PTextFileStream  : public PFile {
        private:
            OpenMode mode_;//ģʽ
            BufferStrategy bufferStrategy_;//�������
            std::shared_mutex shared_mtx_;//��
            std::ifstream ifstream_;//������
            std::ofstream ofstream_;//�����
            ReadWriteMode rw_mode_;

            void SyncReadStream();
        public:
            //������
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
            ˢ�»�����������������д���ļ���Ĭ������»�������ʱ�Զ�ˢ�£�
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
