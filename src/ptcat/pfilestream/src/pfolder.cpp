//
// Created by 27852 on 2025/8/27.
//

#include "ptcat/pfilestream/pfolder.h"
#include <iostream>

namespace ptcat {
    namespace pfilestream {
        PFolder::PFolder(std::string folder)  {
            path_ = fs::path(folder);
            // if (!fs::is_directory(path_)){//这个方法对于已经存在的路径判断是否是文件夹，不符合我的要求
            if (!IsDirectory()){
                throw std::runtime_error("The current path is not a folder");
            }
        }

        PFolder::~PFolder() {

        }

        bool PFolder::IsExists() const {
            return fs::exists(path_);
        }

        bool PFolder::IsExistsAndCreate() {
            if (!IsExists()) {
                return Create();
            }
            return true;
        }

        bool PFolder::IsDirectory() {
            if (IsExists()) {
                return fs::is_directory(path_);
            }
            std::string path = path_.string();
            if (!path.empty() && (path.back() == '/' || path.back() == '\\')) {
                return true;
            }
            if (path_.extension().empty()) {
                return true;
            }
            return false;
        }

        uint64_t PFolder::Size() {
            if (IsExists()) {//如果存在该文件夹，就获取一下文件个数
                std::vector<Path> file_paths;
                GetAllFromFolder(file_paths);
                size_ = file_paths.size();
            }else {
                size_ = 0;
            }
            return size_;
        }

        bool PFolder::Rename(const std::string& new_name) {
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

        bool PFolder::Delete() {
            if (IsExists()) {
                fs::remove_all(path_);
            }
            return true;
        }

        bool PFolder::Create() {
            if (!fs::create_directories(path_)) {//如果有多级， create_directories 会递归创造所有路径，create_directorie 创建多级路径会报错
                return false;
            }
            return true;
        }

        bool PFolder::Move(const std::string& new_path) {
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

        // bool PFolder::Copy(std::string new_pos) {
        //     if (IsExists()) {
        //         //递归拷贝子目录以及覆盖已经存在的文件夹
        //         fs::copy(path_, new_pos, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
        //     }
        //     return false;
        // }

        void PFolder::GetAllFromFolder(std::vector<Path>& file_paths, const std::string& extension) {
            auto its = fs::directory_iterator(path_);
            for (auto& it : its) {
                fs::path path = it.path();
                if (!extension.empty() && path.extension() != extension) {
                    continue;
                }
                file_paths.emplace_back(Path(path.filename().string(), path.string(), path.extension().string()));
            }
            size_ = file_paths.size();
        }
    }
}
