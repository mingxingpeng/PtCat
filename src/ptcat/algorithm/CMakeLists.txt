cmake_minimum_required(VERSION 3.28)
project(PAlgorithm)

set(PTCAT_H_PRIVATE_ALGO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)
set(PTCAT_CPP_ALGO_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/fft/pfft.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/filter/pfilter.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/sort/psort.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/select/pselect.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/common/cat.cpp"
        PARENT_SCOPE
)