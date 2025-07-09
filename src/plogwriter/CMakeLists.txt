
cmake_minimum_required(VERSION 3.28)
project(PLogWriter)

set(PTCAT_H_PRIVATE_LOG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)
set(PTCAT_CPP_LOG_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/logger/plogwriter.cpp" PARENT_SCOPE)



































#cmake_minimum_required(VERSION 3.28)
#project(PLogWriter)
#
#add_library(${PROJECT_NAME} ${PTCAT_CPP_LOG_FILES})#如果头文件和子文件不在一个文件夹中，设置头文件好像会出错，这里尽量别设置头文件
#
##判断一下，如果是静态库，那么需要穿递给 C++ 一个宏，告诉他这个是静态库
#if (NOT PTCAT_LIB_SHARED)
#    #这里写在下面是因为想指定对应传宏的目标，我这里只针对当前库以及其依赖者 add_definitions 也可以，但是其是全局的，比较老
#    target_compile_definitions(${PROJECT_NAME} PUBLIC "STATIC_LIB")
#endif ()
#
#target_include_directories(${PROJECT_NAME} PUBLIC ${PTCAT_H_LOG_LOGGER_DIR} ${PTCAT_H_LOG_CARVE_DIR} ${PTCAT_H_COMMON_DIR})
#
#set_target_properties(${PROJECT_NAME} PROPERTIES
#        OUTPUT_NAME "plogwriter"#设置库的输出名称
#        DEBUG_POSTFIX "d"#如果是 Debug 版本，设置库的后缀
#)

