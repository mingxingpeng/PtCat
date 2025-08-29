//
// Created by 27852 on 2025/8/27.
//
/*
 *目前只是简单封装了一下，后续还需要设置缓存策略等等功能，留个尾巴
 */
#include "ptcat/pfilestream/ptextfilestream.h"
#include <iostream>

namespace ptcat {
    namespace pfilestream {
        void PTextFileStream::Test() {
            if (rw_mode_ == ReadWriteMode::READONLY) {
                auto pposg = ifstream_.tellg();
                std::cout << "read pos " << pposg << std::endl;
            }else if (rw_mode_ == ReadWriteMode::WRITEONLY) {
                auto pposp = ofstream_.tellp();
                std::cout << " write pos " << pposp << std::endl;
            }else {
                auto pposg = ifstream_.tellg();
                auto pposp = ofstream_.tellp();
                std::cout << "read pos " << pposg << " write pos " << pposp << std::endl;
            }

        }

        PTextFileStream::PTextFileStream() {
            mode_ = OpenMode::READWRITE;
            bufferStrategy_ = BufferStrategy::DEFAULT;
        }

        PTextFileStream::PTextFileStream(const std::string& path, OpenMode mode, BufferStrategy bufferStrategy) {
            StatusCode status = Open(path, mode, bufferStrategy);
            if (status == StatusCode::NOT_FILE) {
                throw std::runtime_error("The path is not a file");
            }
            if (status == StatusCode::FILE_NOT_OPEN) {
                throw std::runtime_error("File opening failed");
            }
        }
        PTextFileStream::~PTextFileStream() {
            Close();//关闭流
        }

        //打开文件
        StatusCode PTextFileStream::Open(const std::string& path, OpenMode mode, BufferStrategy bufferStrategy) {
            std::unique_lock<std::shared_mutex> lock(shared_mtx_);
            if (!IsOpen()) {
                path_ = fs::path(path);
                if (IsFile()) {//判断是不是文件
                    mode_ = mode;
                    bufferStrategy_ = bufferStrategy;
                    if (!IsExists()) {
                        //文件不存在时创建
                        Create();
                    }
                    switch (mode_) {
                        case OpenMode::READONLY:
                            ifstream_.open(path_, std::ios::in);
                            rw_mode_ = ReadWriteMode::READONLY;
                            break;
                        case OpenMode::WRITEONLY:
                            ofstream_.open(path_, std::ios::out);
                            rw_mode_ = ReadWriteMode::WRITEONLY;
                            break;
                        case OpenMode::READWRITE:
                            ifstream_.open(path_, std::ios::in);
                            ofstream_.open(path_, std::ios::out);
                            rw_mode_ = ReadWriteMode::DOUBLE;
                            break;
                        case OpenMode::W_APPEND:
                            ofstream_.open(path_, std::ios::out | std::ios::app);
                            rw_mode_ = ReadWriteMode::WRITEONLY;
                            break;
                        case OpenMode::RW_APPEND:
                            ifstream_.open(path_, std::ios::in);
                            ofstream_.open(path_, std::ios::out | std::ios::app);
                            rw_mode_ = ReadWriteMode::DOUBLE;
                            break;
                        case OpenMode::RW_TRUNC:
                            ifstream_.open(path_, std::ios::in);
                            ofstream_.open(path_, std::ios::out | std::ios::trunc);
                            rw_mode_ = ReadWriteMode::DOUBLE;
                            break;
                    }

                    if (!IsOpen()) {
                        return StatusCode::FILE_NOT_OPEN;
                    }else {
                        Test();
                        return StatusCode::SUCCESS;
                    }
                }else {
                    return StatusCode::NOT_FILE;
                }

            }else {
                return StatusCode::CURR_STREAM_IS_OCCUPIED;
            }
        }

        uint64_t PTextFileStream::Size() {//文件尺寸
            if (rw_mode_ == ReadWriteMode::READONLY) {
                auto pos = ifstream_.tellg();
                ifstream_.seekg(0, std::ios::end);
                size_ = ifstream_.tellg();
                ifstream_.seekg(pos);//恢复原始位置
            }else{
                auto pos = ofstream_.tellp();
                ofstream_.seekp(0, std::ios::end);
                size_ = ofstream_.tellp();
                ofstream_.seekp(pos);//恢复原始位置
            }
            Test();
            return size_;
        }

        bool PTextFileStream::IsOpen() {
            if (rw_mode_ == ReadWriteMode::READONLY) {
                return ifstream_.is_open();
            }else if (rw_mode_ == ReadWriteMode::WRITEONLY) {
                return ofstream_.is_open();
            }else {
                return ifstream_.is_open() && ofstream_.is_open();
            }
        }


        void PTextFileStream::Close() {
            std::unique_lock<std::shared_mutex> lock(shared_mtx_);
            if (ifstream_.is_open()) {
                ifstream_.close();
            }
            if (ofstream_.is_open()) {
                ofstream_.close();
            }
        }

        StatusCode PTextFileStream::Flush() {

            if (rw_mode_ == ReadWriteMode::WRITEONLY || rw_mode_ == ReadWriteMode::DOUBLE) {
                if (IsOpen()) {
                    std::unique_lock<std::shared_mutex> lock(shared_mtx_);
                    ofstream_.flush();
                    if (ofstream_.fail()) {
                        ofstream_.clear();
                        return StatusCode::RW_PROCESS_IS_INTERRUPTED;
                    }else {
                        return StatusCode::SUCCESS;
                    }
                }else {
                    return StatusCode::FILE_NOT_OPEN;
                }
            }else {
                return StatusCode::FAILED;
            }
        }

        StatusCode PTextFileStream::Clear() {
            if (IsOpen()) {
                if (rw_mode_ == ReadWriteMode::READONLY) {
                    std::unique_lock<std::shared_mutex> lock(shared_mtx_);
                    ifstream_.clear();
                }else if (rw_mode_ == ReadWriteMode::WRITEONLY) {
                    std::unique_lock<std::shared_mutex> lock(shared_mtx_);
                    ofstream_.clear();
                }else {
                    std::unique_lock<std::shared_mutex> lock(shared_mtx_);
                    ifstream_.clear();
                    ofstream_.clear();
                }
                return StatusCode::SUCCESS;
            }else {
                return StatusCode::FILE_NOT_OPEN;
            }
        }

        StatusCode PTextFileStream::SeekPos(uint64_t offest, Pos pos, ReadWriteMode pos_mode) {
            if (IsOpen()) {
                bool is_success = false;
                auto status = pos == Pos::BEGIN ? std::ios::beg : std::ios::end;
                if (rw_mode_ == ReadWriteMode::READONLY
                    || (rw_mode_ == ReadWriteMode::DOUBLE && pos_mode == ReadWriteMode::READONLY)
                    || (rw_mode_ == ReadWriteMode::DOUBLE && pos_mode == ReadWriteMode::DOUBLE)) {
                    ifstream_.seekg(offest, status);
                    Test();
                    if (ifstream_.eof()) {
                        ifstream_.clear();
                        is_success = true;
                    }else {
                        if (ifstream_.fail()) {
                            ifstream_.clear();
                            ifstream_.seekg(0, status);
                        }else {
                            is_success = true;
                        }
                    }
                }
                if (rw_mode_ == ReadWriteMode::WRITEONLY
                    || (rw_mode_ == ReadWriteMode::DOUBLE && pos_mode == ReadWriteMode::WRITEONLY)
                    || (rw_mode_ == ReadWriteMode::DOUBLE && pos_mode == ReadWriteMode::DOUBLE)) {
                    ofstream_.seekp(offest, status);
                    Test();
                    if (ofstream_.eof()) {
                        ofstream_.clear();
                        is_success = true;
                    }else {
                        if (ofstream_.fail()) {
                            ofstream_.clear();
                        }else {
                            is_success = true;
                        }
                    }
                }
                if (!is_success) return StatusCode::FAILED;
            }else {
                return StatusCode::FILE_NOT_OPEN;
            }
        }

        void PTextFileStream::SyncReadStream() {
            if (ifstream_.is_open()) {
                // 清除读取流错误状态
                ifstream_.clear();
                // 重新打开读取流（确保获取最新文件内容）
                auto current_r_pos = ifstream_.tellg();  // 保存当前读指针位置
                ifstream_.close();
                ifstream_.open(path_, std::ios::in);      // 重新打开

                // 尝试恢复到之前的读指针位置（如果位置有效）
                if (ifstream_.is_open()) {
                    if (current_r_pos >= 0) {
                        ifstream_.seekg(current_r_pos);
                        // 如果原位置超出文件新长度，则调整到文件末尾
                        if (ifstream_.fail()) {
                            ifstream_.clear();
                            ifstream_.seekg(0, std::ios::end);
                        }
                    }else {
                        ifstream_.seekg(0, std::ios::end);
                    }

                }
            }
        }

        StatusCode PTextFileStream::ReadAll(std::string& content) {
            if (rw_mode_ == ReadWriteMode::WRITEONLY) {
                return StatusCode::FAILED;
            }
            content.clear();
            std::shared_lock<std::shared_mutex> lock(shared_mtx_);
            if (IsOpen()) {
                SyncReadStream();//同步更新一下
                uint64_t size = Size();
                if (size > 0) {
                    content.resize(static_cast<size_t>(size));
                    SeekPos(0, Pos::BEGIN, ReadWriteMode::READONLY);
                    ifstream_.read(content.data(), size);
                    if (ifstream_.eof()) {
                        ifstream_.clear();
                        Test();
                        return StatusCode::SUCCESS;
                    }else {
                        if (ifstream_.fail()) {//? eof ??????? true, ??????????????????
                            ifstream_.clear();
                            Test();
                            return StatusCode::RW_PROCESS_IS_INTERRUPTED;
                        }else {
                            Test();
                            return StatusCode::SUCCESS;
                        }
                    }
                }
            }else {
                return StatusCode::FILE_NOT_OPEN;
            }
            return StatusCode::FAILED;
        }

        StatusCode PTextFileStream::ReadLine(std::string& line) {
            if (rw_mode_ == ReadWriteMode::WRITEONLY) {
                return StatusCode::FAILED;
            }
            std::shared_lock<std::shared_mutex> lock(shared_mtx_);
            line.clear();
            if (IsOpen()) {
                SyncReadStream();//同步更新一下
                if (std::getline(ifstream_, line)) {
                    Test();
                    return StatusCode::SUCCESS;
                }else {
                    if (ifstream_.eof()) {
                        ifstream_.clear();
                        Test();
                        return StatusCode::FILE_EOF;
                    }else {
                        ifstream_.clear();
                        Test();
                        return StatusCode::RW_PROCESS_IS_INTERRUPTED;
                    }
                }
            }else {
                return StatusCode::FILE_NOT_OPEN;
            }
        }

        StatusCode PTextFileStream::WriteAll(const std::string& content) {
            std::unique_lock<std::shared_mutex> lock(shared_mtx_);
            if (rw_mode_ == ReadWriteMode::READONLY) {
                return StatusCode::FAILED;
            }
            if (IsOpen()) {
                //获取到读指针位置
                ofstream_ << content;
                ofstream_.flush();
                if (ofstream_.fail()) {
                    ofstream_.clear();
                    Test();
                    return StatusCode::RW_PROCESS_IS_INTERRUPTED;
                }else {
                    Test();
                    return StatusCode::SUCCESS;
                }
            }else {
                return StatusCode::FILE_NOT_OPEN;
            }
        }

        StatusCode PTextFileStream::WriteLine(const std::string& line, bool add_new_line) {
            std::unique_lock<std::shared_mutex> lock(shared_mtx_);
            if (rw_mode_ == ReadWriteMode::READONLY) {
                return StatusCode::FAILED;
            }
            if (IsOpen()) {

                ofstream_.flush();
                ofstream_ << line;
                if (add_new_line) {
                    ofstream_ << std::endl;
                }
                ofstream_.flush();
                if (ofstream_.fail()) {
                    ofstream_.clear();
                    Test();
                    return StatusCode::RW_PROCESS_IS_INTERRUPTED;
                }else {
                    Test();
                    return StatusCode::SUCCESS;
                }
            }else {
                return StatusCode::FILE_NOT_OPEN;
            }
        }
    }
}
