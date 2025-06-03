cmake_minimum_required(VERSION 3.28)

#**********************************************
#为了以后便于维护以及相应官方号召， 下面我将不在使用 aux_source_directory 或 file 来获取到指定文件夹中的所有 cpp 或 h 文件
#**********************************************


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#**********************************************
#开始程序
#**********************************************
#载入工具类
include(cmake/utils.cmake)
include(cmake/out_dir.cmake)
ptcat_extract_version()#设置版本
output_dir()#设置文件生成位置

#**********************************************
#指定项目名称，项目版本，以及所使用的语言
#**********************************************
project(ptcat)
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
#是否只生成对应的库还是生成总库
#**********************************************
#option(PTCAT_GENERATE_ALL_LIBRARY "build all libraries" OFF)
#如果不生成总库，对应库生成， 对应库我发现只会生成最后面一个，去除该功能
#option(PTCAT_THREADPOOL_LIBRARY "build pthreadpool libraries" ON)#线程池库
#option(PTCAT_LOGWRITER_LIBRARY "build plogwriter libraries" ON)#日志库
#option(PTCAT_CLOCK_LIBRARY "build pclock libraries" ON)#时间库

#**********************************************
#头文件和源文件集合
#**********************************************
set(PTCAT_H_ALL_DIRS "")#所有的头文件
set(PTCAT_H_PRIVATE_DIRS "")#所有私有的头文件
set(PTCAT_CPP_ALL_FILES "")#所有涉及到的的 cpp 文件

#**********************************************
#公共库地址
#**********************************************
set(PTCAT_H_COMMON_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

#**********************************************
#线程池库设置
#**********************************************
#设置线程池前头文件地址和源文件地址，并且获得所有的头文件和源文件
set(PTCAT_H_TPOOL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
#设置线程池源文件地址
#set(PTCAT_CPP_TPOOL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/pthreadpool/src")
#include_directories(PTCAT_H_TPOOL_DIR)#头文件引用带有全局性质，如果其含有子类，可能会有问题，使用下面的 target_include_directories来为当前 cmake 和其依赖者设置头文件路径
#获取到线程池的所有源文件
#aux_source_directory(${PTCAT_CPP_TPOOL_DIR} PTCAT_CPP_TPOOL_FILES)#根据指定的路径，获取到该路径所有的 cpp 文件
set(PTCAT_CPP_TPOOL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/pthreadpool/src/pthreadpool.cpp")
#将头文件与 cpp 文件添加至 PTCAT_H_ALL_DIRS 与 PTCAT_CPP_ALL_FILES 存储起来
#第一行的数据不需要加上间隔符
string(APPEND PTCAT_H_ALL_DIRS "" ${PTCAT_H_TPOOL_DIR})
list(APPEND PTCAT_CPP_ALL_FILES ${PTCAT_CPP_TPOOL_FILES})

#**********************************************
#日志库设置
#**********************************************
#设置线程池前头文件地址和源文件地址，并且获得所有的头文件和源文件
set(PTCAT_H_LOG_LOGGER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(PTCAT_H_LOG_CARVE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
#设置日志源文件地址
#set(PTCAT_CPP_LOG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/plogwriter/logger/src")
#aux_source_directory(${PTCAT_CPP_LOG_DIR} PTCAT_CPP_LOG_FILES)#获取到日志源文件
set(PTCAT_CPP_LOG_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/plogwriter/src/logger/plogwriter.cpp")
#头文件汇总
string(APPEND PTCAT_H_ALL_DIRS ";" ${PTCAT_H_LOG_LOGGER_DIR})
string(APPEND PTCAT_H_ALL_DIRS ";" ${PTCAT_H_LOG_CARVE_DIR})
list(APPEND PTCAT_CPP_ALL_FILES ${PTCAT_CPP_LOG_FILES})

#**********************************************
#时间库设置
#**********************************************
set(PTCAT_H_CLOCK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(PTCAT_H_PRIVATE_CLOCK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/pclock/include")
#set(PTCAT_CPP_DATETIME_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/pdatetime/src")
#获取到所有在 PTCAT_CPP_DATETIME_DIR 中的 cpp 文件保存到变量 PTCAT_CPP_DATETIME_FILES 中
#aux_source_directory(${PTCAT_CPP_DATETIME_DIR} PTCAT_CPP_DATETIME_FILES)
set(PTCAT_CPP_CLOCK_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pclock/src/pstopwatch.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pclock/src/pchronograph.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pclock/src/pdatetime.cpp")
#将头文件与 cpp 文件汇总
#[[
string(APPEND PTCAT_CPP_ALL_FILES ";" ${PTCAT_CPP_DATETIME_FILES})
展开后等效于
string(APPEND PTCAT_CPP_ALL_FILES ";" "E:/path/test.cpp" "E:/path/test1.cpp")
原本变量中的字符串因为 cmake 的规则给当成分隔符去掉了，所以这里建议
string(APPEND PTCAT_CPP_ALL_FILES ";${PTCAT_CPP_DATETIME_FILES}")
或者直接使用 list(APPEND)
]]
string(APPEND PTCAT_H_ALL_DIRS ";" ${PTCAT_H_CLOCK_DIR})#这里就不改了，还是用 string
string(APPEND PTCAT_H_PRIVATE_DIRS ";" ${PTCAT_H_PRIVATE_CLOCK_DIR})#私有头文件
list(APPEND PTCAT_CPP_ALL_FILES  ${PTCAT_CPP_CLOCK_FILES})

#**********************************************
#展示汇总的数据
#**********************************************
message("-- -- PTCAT_H_ALL_DIRS = ${PTCAT_H_ALL_DIRS}")
message("-- -- PTCAT_CPP_ALL_FILES = ${PTCAT_CPP_ALL_FILES}")

#**********************************************
#创建动态库还是静态库,设置是生成动态库还是静态库 ON 是生成动态库， OFF 是生成静态库
#**********************************************
option(PTCAT_LIB_SHARED "create shared lib or static lib" ON)#注意这里如果是设置为了动态库，需要对应头文件加 dllexport, 否则生成不了 .lib 函数地址文件
set(BUILD_SHARED_LIBS ${PTCAT_LIB_SHARED})

#**********************************************
#用于关联库
#**********************************************
set(PTCAT_LIB_PROJECT_NAMES ${PROJECT_NAME})#默认当前库

#**********************************************
#是否生成所有库还是只生成对应库
#**********************************************
#if (PTCAT_GENERATE_ALL_LIBRARY)
#    add_library(${PROJECT_NAME} ${PTCAT_CPP_ALL_FILES})
#    #判断一下，如果是静态库，那么需要穿递给 C++ 一个宏，告诉他这个是静态库
#    if (NOT PTCAT_LIB_SHARED)
#        #这里写在下面是因为想指定对应传宏的目标，我这里只针对当前库以及其依赖者 add_definitions 也可以，但是其是全局的，比较老
#        target_compile_definitions(${PROJECT_NAME} PRIVATE "STATIC_LIB")
#    endif ()
#    target_include_directories(${PROJECT_NAME}
#            PUBLIC ${PTCAT_H_ALL_DIRS} ${PTCAT_H_COMMON_DIR}
#            PRIVATE ${PTCAT_H_PRIVATE_DIRS})#设置私有头文件
#
#    #设置库的属性
#    set_target_properties(${PROJECT_NAME} PROPERTIES
#                            OUTPUT_NAME "ptcat"#设置库的输出名称
#                            DEBUG_POSTFIX "d"#如果是 Debug 版本，设置库的后缀
#    )
#else()
#    set(PTCAT_LIB_PROJECT_NAMES "")
#    #单独编译线程池库
#    if (PTCAT_THREADPOOL_LIBRARY)
#        add_subdirectory(src/pthreadpool)
#        string(APPEND PTCAT_LIB_PROJECT_NAMES "" src/pthreadpool)
#    endif ()
#    #单独编译日志库
#    if (PTCAT_LOGWRITER_LIBRARY)
#        add_subdirectory(src/plogwriter)
#        string(APPEND PTCAT_LIB_PROJECT_NAMES ";" src/plogwriter)
#    endif ()
#    #单独编译时间库
#    if (PTCAT_CLOCK_LIBRARY)
#        add_subdirectory(src/pclock)
#        string(APPEND PTCAT_LIB_PROJECT_NAMES ";" src/pclock)
#    endif ()
#endif ()

add_library(${PROJECT_NAME} ${PTCAT_CPP_ALL_FILES})
#判断一下，如果是静态库，那么需要穿递给 C++ 一个宏，告诉他这个是静态库
if (NOT PTCAT_LIB_SHARED)
    #这里写在下面是因为想指定对应传宏的目标，我这里只针对当前库以及其依赖者 add_definitions 也可以，但是其是全局的，比较老
    target_compile_definitions(${PROJECT_NAME} PRIVATE "STATIC_LIB")
endif ()
target_include_directories(${PROJECT_NAME}
        PUBLIC ${PTCAT_H_ALL_DIRS} ${PTCAT_H_COMMON_DIR}
        PRIVATE ${PTCAT_H_PRIVATE_DIRS})#设置私有头文件

#设置库的属性
set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "ptcat"#设置库的输出名称
        DEBUG_POSTFIX "d"#如果是 Debug 版本，设置库的后缀
)
message("-- -- PTCAT_LIB_PROJECT_NAMES = ${PTCAT_LIB_PROJECT_NAMES}")


#**********************************************
#是否运行示例
#**********************************************
if (PTCAT_RUN_EXAMPLE)
    add_subdirectory(example)
endif ()










