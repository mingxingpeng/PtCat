cmake_minimum_required(VERSION 3.28)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#**********************************************
#开始程序
#**********************************************
#载入工具类
include(cmake/utils.cmake)
include(cmake/out_dir.cmake)
ptcat_extract_version()
output_dir()


#指定下项目名称，项目版本，以及所使用的语言
project(PtCat)
message(STATUS "PtCat VERSION ${PTCAT_VERSION}")

#**********************************************
#设置默认编译状态为 Release 状态
#**********************************************
#if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)#如果没有给 CMAKE_BUILD_TYPE 赋值，或者没有设置多配置生成器 设置 CMAKE_BUILD_TYPE 为 Release 模式
#    message(STATUS "Release")
#    set(CMAKE_BUILD_TYPE "Release" CACHE STRING  "Choose Release or Debug" FORCE)
#endif()

#**********************************************
#设置是否使用编译器扩展功能
#**********************************************
set(CMAKE_CXX_EXTENSIONS OFF)
if (CMAKE_SYSTEM_NAME MATCHES "CYGWIN" OR CMAKE_SYSTEM_NAME MATCHES "MSYS" OR CMAKE_SYSTEM_NAME MATCHES "MINGW")
    set(CMAKE_CXX_EXTENSIONS ON)
endif ()

#**********************************************
#是否调用 PtCat 库测试
#**********************************************
option(PTCAT_RUN_EXAMPLE "run the example" ON)

#**********************************************
#是否只生成对应的库还是生成全部
#**********************************************
option(PTCAT_ALL_LIBRARY "build all libraries" ON)
if (PTCAT_ALL_LIBRARY)
    option(PTCAT_THREADPOOL_LIBRARY "build pthreadpool libraries" OFF)
    option(PTCAT_LOGWRITER_LIBRARY "build plogwriter libraries" OFF)
else ()
    option(PTCAT_THREADPOOL_LIBRARY "build pthreadpool libraries" ON)
    option(PTCAT_LOGWRITER_LIBRARY "build plogwriter libraries" ON)
endif ()

#**********************************************
#头文件和源文件集合
#**********************************************
set(H_DIRS "")
set(CPP_FILES "")

#**********************************************
#线程池库设置
#**********************************************
#设置线程池前头文件地址和源文件地址，并且获得所有的头文件和源文件
set(TPOOL_H_DIR "${CMAKE_CURRENT_SOURCE_DIR}/pthreadpool/include")
#设置线程池源文件地址
set(TPOOL_CPP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/pthreadpool/src")
#include_directories(TPOOL_H_DIR)#头文件引用带有全局性质，如果其含有子类，可能会有问题，使用下面的 target_include_directories来为当前 cmake 和其依赖者设置头文件路径
#获取到线程池的所有源文件
aux_source_directory(${TPOOL_CPP_DIR} TPOOL_CPP_FILES)
string(APPEND H_DIRS "" ${TPOOL_H_DIR})
string(APPEND CPP_FILES "" ${TPOOL_CPP_FILES})

#**********************************************
#日志库设置
#**********************************************
#设置线程池前头文件地址和源文件地址，并且获得所有的头文件和源文件
set(LOG_H_DIR "${CMAKE_CURRENT_SOURCE_DIR}/plogwriter/include")
#设置线程池源文件地址
set(LOG_CPP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/plogwriter/src")
#include_directories(TPOOL_H_DIR)#头文件引用带有全局性质，如果其含有子类，可能会有问题，使用下面的 target_include_directories来为当前 cmake 和其依赖者设置头文件路径
#获取到线程池的所有源文件
aux_source_directory(${LOG_CPP_DIR} LOG_CPP_FILES)
string(APPEND H_DIRS ";" ${LOG_H_DIR})
string(APPEND CPP_FILES ";" ${LOG_CPP_FILES})

message("-- -- H_DIRS = ${H_DIRS}")
message("-- -- CPP_FILES = ${CPP_FILES}")

#**********************************************
#创建动态库还是静态库,设置是生成动态库还是静态库 ON 是生成动态库， OFF 是生成静态库
#**********************************************
option(LIB_SHARED "create shared lib or static lib" ON)#注意这里如果是设置为了动态库，需要对应头文件加 dllexport, 否则生成不了 .lib 函数地址文件
set(BUILD_SHARED_LIBS ${LIB_SHARED})

#**********************************************
#用于关联库
#**********************************************
set(LIB_PROJECT_NAMES ${PROJECT_NAME})#默认当前库

#**********************************************
#是否生成所有库还是只生成对应库
#**********************************************


if (PTCAT_ALL_LIBRARY)
    add_library(${PROJECT_NAME} ${CPP_FILES})
    #判断一下，如果是静态库，那么需要穿递给 C++ 一个宏，告诉他这个是静态库
    if (NOT LIB_SHARED)
        #这里写在下面是因为想指定对应传宏的目标，我这里只针对当前库以及其依赖者 add_definitions 也可以，但是其是全局的，比较老
        target_compile_definitions(${PROJECT_NAME} PUBLIC "STATIC_LIB")
    endif ()
    target_include_directories(${PROJECT_NAME} PUBLIC ${H_DIRS})
else()
    set(LIB_PROJECT_NAMES "")
    #添加线程池库
    if (PTCAT_THREADPOOL_LIBRARY)
        add_subdirectory(pthreadpool)
        string(APPEND LIB_PROJECT_NAMES "" PThreadPool)
    endif ()
    #添加日志库
    if (PTCAT_LOGWRITER_LIBRARY)
        add_subdirectory(plogwriter)
        string(APPEND LIB_PROJECT_NAMES ";" PLogWriter)
    endif ()
endif ()
message("-- -- LIB_PROJECT_NAMES = ${LIB_PROJECT_NAMES}")

#**********************************************
#是否运行示例
#**********************************************
if (PTCAT_RUN_EXAMPLE)
    add_subdirectory(example)
endif ()










