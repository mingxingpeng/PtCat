//
// Created by 27852 on 2025/8/27.
//

#include "ptcat/pfilestream/pfile.h"

namespace ptcat {
    namespace pfilestream {

        PFile::PFile() {
        }

        PFile::PFile(const std::string& path) {
            if (!View(path)) {
                throw std::runtime_error("The path is not a file");
            }
        }
        PFile::~PFile() {

        }

        bool PFile::View(const std::string& path) {
            path_ = fs::path(path);
            if (!IsFile()){
                return false;
            }
            return true;
        }

        bool PFile::IsFile() {
            if (IsExists()) {//如果文件存在，直接用该方法判断是否为文件
                return fs::is_regular_file(path_);
            }
            if (path_.extension().empty()) {//判断是否有后缀
                return false;
            }
            std::string path = path_.string();
            if (path.back() == '/' || path.back() == '\\') {//说明是文件夹
                return false;
            }
            //有其他情况后续加
            return true;
        }


        bool PFile::IsExists() const {
            return fs::exists(path_);
        }

        uint64_t PFile::Size() {
            try {
                if (IsExists()) {
                    return fs::file_size(path_);
                }
            } catch (...) {
                return 0;
            }
        }

        bool PFile::Rename(const std::string& new_name) {
            try {
                if (IsExists()) {
                    std::string new_path = path_.parent_path().string();
                    if (new_path.back() == '/' || new_path.back() == '\\') {
                        new_path.append(new_name);
                    }else {
                        new_path.append("/").append(new_name);
                    }
                    fs::rename(path_, new_path);
                }else {
                    return false;
                }
                return true;
            }catch(...) {
                return false;
            }
        }

        bool PFile::Delete() {
            if (IsExists()) {
                fs::remove(path_);
            }
            return true;
        }

        bool PFile::Create() {
            fs::path file_path = path_.parent_path();
            //判断下父类路径在不在
            if (!fs::exists(file_path)) {
                fs::create_directories(file_path);
            }
            std::ofstream file(path_.string());
            if (file) {
                file.close();
                return true;
            }else {
                return false;
            }
            return true;
        }

        bool PFile::Move(const std::string& new_path) {
            try {//原理就是改变文件夹的前缀，就是改变其位置
                if (IsExists()) {
                    std::string path_name = path_.filename().string();
                    std::string move_path = "";//移动路径
                    move_path.append(new_path);
                    if (new_path.back() == '/' || new_path.back() == '\\') {
                        move_path.append(path_name);
                    }else {
                        move_path.append("/").append(path_name);
                    }
                    fs::rename(path_, move_path);
                }else {
                    return false;
                }
                return true;
            }catch (...) {
                return false;
            }
        }

        bool PFile::Copy(const std::string& new_pos) {
            try {//原理就是改变文件夹的前缀，就是改变其位置
                if (IsExists()) {
                    std::string path_name = path_.filename().string();
                    std::string move_path = "";//移动路径
                    move_path.append(new_pos);

                    if (new_pos.back() == '/' || new_pos.back() == '\\') {
                        move_path.append(path_name);
                    }else {
                        move_path.append("/").append(path_name);
                    }
                    fs::copy_file(path_, move_path, fs::copy_options::overwrite_existing);
                }else {
                    return false;
                }
                return true;
            }catch (...) {
                return false;
            }
        }
    }
}