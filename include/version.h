//
// Created by pmx on 2025/3/10.
// 版本头文件
//

#ifndef VERSION_H
#define VERSION_H
#define PTCAT_VERSION_MAJOR 1
#define PTCAT_VERSION_MINOR 1
#define PTCAT_VERSION_PATCH 1

#// 计算头文件号
#define PTCAT_TO_VERSION(major, minor, patch) (major * 10000 + minor * 100 + patch)
#define PTCAT_VERSION SPDLOG_TO_VERSION(PTCAT_VERSION_MAJOR, PTCAT_VERSION_MINOR, PTCAT_VERSION_PATCH)


#endif //VERSION_H
