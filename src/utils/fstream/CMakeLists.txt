cmake_minimum_required(VERSION 3.28)
project(PFstream)

#设置线程池源文件地址
set(PTCAT_CPP_FSTREAM_FILES "${CMAKE_CURRENT_SOURCE_DIR}/pfstream.cpp"
                            "${CMAKE_CURRENT_SOURCE_DIR}/pbinarystream.cpp"
                            "${CMAKE_CURRENT_SOURCE_DIR}/ptextstream.cpp"
        PARENT_SCOPE)