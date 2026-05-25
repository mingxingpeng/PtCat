//
// Created by 27852 on 2026/5/25.
//
//字符串常用工具类，后续会根据实际情况添加

#include "ptcat/utils/string/pstring.h"
#include <sstream>
#include <algorithm>

namespace ptcat{
    namespace string{
        void Split(const std::string& str, char delimiter, std::vector<std::string>& res){
            res.clear();
            std::stringstream ss(str);
            std::string s;
            while(std::getline(ss, s, delimiter)){
                if (!s.empty())
                    res.emplace_back(s);
            }
        }

        void ToUpper(std::string& str){
            std::transform(str.begin(), str.end(), str.begin(), std::toupper);
        }

        void ToUpper(const std::string& str, std::string& res){
            res.clear();
            res.resize(str.length());
            std::transform(str.begin(), str.end(), res.begin(), std::toupper);
        }

        void ToLower(std::string& str){
            std::transform(str.begin(), str.end(), str.begin(), std::tolower);
        }

        void ToLower(const std::string& str, std::string& res){
            res.clear();
            res.resize(str.length());
            std::transform(str.begin(), str.end(), res.begin(), std::tolower);
        }

        void Trim( std::string& str){
            auto start = str.find_first_not_of(WhiteSpace);//寻找头部不是空格的位置
            if (start == std::string::npos) {
                str = "";//全部是空格
                return;
            }

            auto end = str.find_last_not_of(WhiteSpace);//寻找尾部不是空格的位置
            str = str.substr(start, end - start + 1);
        }

        void Trim(const std::string& str, std::string& res){
            auto start = str.find_first_not_of(WhiteSpace);//寻找头部不是空格的位置
            if (start == std::string::npos) {
                res = "";//全部是空格
                return;
            }
            auto end = str.find_last_not_of(WhiteSpace);//寻找尾部不是空格的位置
            res = str.substr(start, end - start + 1);
        }

        //去除开头空格
        void TrimBegin(std::string& str){
            auto start = str.find_first_not_of(WhiteSpace);
            if (start == std::string::npos) {
                str = "";//全部是空格
                return;
            }
            str.erase(0, start);
        }

        void TrimBegin(const std::string& str, std::string& res){
            auto start = str.find_first_not_of(WhiteSpace);
            if (start == std::string::npos) {
                res = "";//全部是空格
                return;
            }
            res = str.substr(start);
        }

        void TrimEnd(std::string& str){
            auto end = str.find_last_not_of(WhiteSpace);//寻找尾部不是空格的位置
            if (end == std::string::npos) {
                str = "";//全部是空格
                return;
            }
            str.erase(end + 1);
        }

        void TrimEnd(const std::string& str, std::string& res){
            auto end = str.find_last_not_of(WhiteSpace);//寻找尾部不是空格的位置
            if (end == std::string::npos) {
                res = "";//全部是空格
                return;
            }
            res = str.substr(0, end + 1);
        }

        bool Contains(const std::string& str, const std::string& sub_str){
            return str.find(sub_str) != std::string::npos;
        }

        bool StartsWith(const std::string& str, const std::string& prefix){
            int str_len= str.length();
            int prefix_len= prefix.length();
            if (prefix_len > str_len) return false;
            return str.compare(0, prefix_len, prefix) == 0;
        }

        bool EndsWith(const std::string& str, const std::string& suffix){
            int str_len= str.length();
            int prefix_len= suffix.length();
            if (prefix_len > str_len) return false;
            return str.compare(str_len - prefix_len, prefix_len, suffix) == 0;
        }
    }
}