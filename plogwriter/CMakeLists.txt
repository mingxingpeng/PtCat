cmake_minimum_required(VERSION 3.28)
project(PLogWriter)

add_library(${PROJECT_NAME} ${LOG_CPP_FILES})#如果头文件和子文件不在一个文件夹中，设置头文件好像会出错，这里尽量别设置头文件

#判断一下，如果是静态库，那么需要穿递给 C++ 一个宏，告诉他这个是静态库
if (NOT LIB_SHARED)
    #这里写在下面是因为想指定对应传宏的目标，我这里只针对当前库以及其依赖者 add_definitions 也可以，但是其是全局的，比较老
    target_compile_definitions(${PROJECT_NAME} PUBLIC "STATIC_LIB")
endif ()

target_include_directories(${PROJECT_NAME} PUBLIC ${LOG_H_DIR})
