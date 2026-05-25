//
// Created by 27852 on 2026/5/25.
//

#ifndef PTCAT_PSTRING_H
#define PTCAT_PSTRING_H

#include "ptcat/common/common.h"
#include <string>
#include <vector>
namespace ptcat{
    namespace string{
        const static std::string WhiteSpace = " \t\n\r\f\v";//空格

        //根据分隔符分割数据
        inline DLL_API void Split(const std::string& str, char delimiter, std::vector<std::string>& res);

        //字符串放大
        inline DLL_API void ToUpper(std::string& str);

        inline DLL_API void ToUpper(const std::string& str, std::string& res);

        //字符串缩小
        inline DLL_API void ToLower(std::string& str);

        inline DLL_API void ToLower(const std::string& str, std::string& res);

        //去除首尾空格
        inline DLL_API void Trim(std::string& str);

        inline DLL_API void Trim(const std::string& str, std::string& res);

        //去除开头空格
        inline DLL_API void TrimBegin(std::string& str);

        inline DLL_API void TrimBegin(const std::string& str, std::string& res);

        //去除末尾空格
        inline DLL_API void TrimEnd(std::string& str);

        inline DLL_API void TrimEnd(const std::string& str, std::string& res);

        //判断是否包含某个字符串
        inline DLL_API bool Contains(const std::string& str, const std::string& sub_str);

        //判断是否以指定字符开头结尾
        inline DLL_API bool StartsWith(const std::string& str, const std::string& prefix);

        inline DLL_API bool EndsWith(const std::string& str, const std::string& suffix);

    }
}

#endif //PTCAT_PSTRING_H
