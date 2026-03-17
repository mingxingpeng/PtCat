cmake_minimum_required(VERSION 3.28)
project(PAlgorithm)

set(PTCAT_H_PRIVATE_ALGO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)
set(PTCAT_CPP_ALGO_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/fft/pfft.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/filter/pfilter.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/sort/psort.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/select/pselect.cpp"
        PARENT_SCOPE
)