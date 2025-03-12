function(output_dir)
    #设置一下静态库，动态库的各个文件的位置, CMAKE_CURRENT_LIST_DIR 可能会受到 .cmake 文件调用的影响，因为他也会获取 .cmake 文件所在的路径
    #message(STATUS "** MAIN CMAKE_CURRENT_LIST_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")
    #创建库目录
    set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
    set(BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")
    #message(STATUS "** MAIN BIN_DIR : ${BIN_DIR}")

    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR} PARENT_SCOPE)#只针对 linux 动态库
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR} PARENT_SCOPE)#针对 windows 静态库，windows 动态库 .lib 函数路径， linux 静态库
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR} PARENT_SCOPE)#针对 windows 动态库 dll 文件;windows、linux 执行程序，.pbd 调试信息文件
endfunction()
