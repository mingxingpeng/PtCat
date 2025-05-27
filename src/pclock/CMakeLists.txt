cmake_minimum_required(VERSION 3.28)
project(PClock)

#获取到对应路径下的所有的头文件
#file(GLOB HEARD_FILES "${PTCAT_H_TPOOL_DIR}/*.h*")
#message(STATUS "** HEARD_FILES : ${HEARD_FILES}")

add_library(${PROJECT_NAME} ${PTCAT_CPP_CLOCK_FILES})#如果头文件和子文件不在一个文件夹中，设置头文件好像会出错，这里尽量别设置头文件

#判断一下，如果是静态库，那么需要穿递给 C++ 一个宏，告诉他这个是静态库
if (NOT PTCAT_LIB_SHARED)
    #这里写在下面是因为想指定对应传宏的目标，我这里只针对当前库以及其依赖者 add_definitions 也可以，但是其是全局的，比较老
    target_compile_definitions(${PROJECT_NAME} PUBLIC "STATIC_LIB")
endif ()

#[[
    为对应目标以及其依赖者设置头文件地址，例如 pthreadpool.cpp 就是其依赖者
        add_library 设置目标以及这个目标的依赖者，所以需要 target_include_directories 要写在 add_library 的后面
        同理 target_link_directories 也一样
]]
target_include_directories(${PROJECT_NAME} PUBLIC ${PTCAT_H_CLOCK_DIR} ${PTCAT_H_COMMON_DIR})


