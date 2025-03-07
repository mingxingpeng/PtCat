cmake_minimum_required(VERSION 3.28)
project(PtCat)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#设置一下静态库，动态库的各个文件的位置, CMAKE_CURRENT_LIST_DIR 可能会受到 .cmake 文件调用的影响，因为他也会获取 .cmake 文件所在的路径
#message(STATUS "** MAIN CMAKE_CURRENT_LIST_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")
#创建库目录
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
#message(STATUS "** MAIN LIB_DIR : ${LIB_DIR}")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")#只针对 linux 动态库
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})#针对 windows 静态库，windows 动态库 .lib 函数路径， linux 静态库
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")#针对 windows 动态库 dll 文件;windows、linux 执行程序，.pbd 调试信息文件

#设置线程池前头文件地址
set(TPOOL_H_DIR "${CMAKE_CURRENT_SOURCE_DIR}/pthreadpool/include")

#添加线程池，其自定义生成库
add_subdirectory(pthreadpool)

#message(STATUS "** MAIN project name : ${PROJECT_NAME}")#指定下等级，--log-level=VERBOSE 可以看到
add_executable(${PROJECT_NAME} main.cpp)

#赋予库地址
#target_link_directories(${PROJECT_NAME} PRIVATE ${LIB_DIR})#最新版要指定作用域
#关联下对应的头文件
target_include_directories(${PROJECT_NAME} PUBLIC ${TPOOL_H_DIR})#之让其依赖项（例如：main.cpp） 可以获取到头文件
#关联对应的库
target_link_libraries(${PROJECT_NAME} PUBLIC PThreadPool)







