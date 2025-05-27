cmake_minimum_required(VERSION 3.28)
project(PtCat_examples)

#message(STATUS "** MAIN project name : ${PROJECT_NAME}")#指定下等级，--log-level=VERBOSE 可以看到
add_executable(${PROJECT_NAME} example.cpp)

#关联下对应的头文件
target_include_directories(${PROJECT_NAME} PUBLIC ${PTCAT_H_ALL_DIRS})#之让其依赖项（例如：main.cpp） 可以获取到头文件
message("PTCAT_H_ALL_DIRS = ${PTCAT_H_ALL_DIRS}")
#
##关联对应的库
target_link_libraries(${PROJECT_NAME} PUBLIC ${PTCAT_LIB_PROJECT_NAMES})
message("PTCAT_LIB_PROJECT_NAMES = ${PTCAT_LIB_PROJECT_NAMES}")