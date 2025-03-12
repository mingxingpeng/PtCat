#retrive the current version
function(ptcat_extract_version)
    #获取到 version.h 的头文件
    file(READ ${CMAKE_CURRENT_LIST_DIR}/include/version.h ptcat_version)# CMAKE_CURRENT_LIST_DIR 用于获取 cmake 的根目录

    #读取到对应的字符串文件
    string(REGEX MATCH "PTCAT_VERSION_MAJOR ([0-9]+)" _ "${ptcat_version}")#这里使用正则表达式从 ptcat_version 检索将检测结果存储到 _，但实际上是存储到 CMAKE_MATCH_0,CMAKE_MATCH_1,CMAKE_MATHC_2 中
    #使用 CMAKE_MATCH_COUNT 来判断是否获取到正则表达式所匹配的结果，然后使用其对应的 CMAKE_MATCH_0,CMAKE_MATCH_1,CMAKE_MATHC_2 来获取真正的结果
    if (NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "could not extract major version number from PtCat/include/version.h")
    endif ()
    set(var_major ${CMAKE_MATCH_1})

    string(REGEX MATCH "PTCAT_VERSION_MINOR ([0-9]+)" _ ${ptcat_version})
    if (NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "could not extract minor version number from PtCat/include/version.h")
    endif ()
    set(var_minor ${CMAKE_MATCH_1})

    string(REGEX MATCH "PTCAT_VERSION_PATCH ([0-9]+)" _ ${ptcat_version})
    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "could not extract patch version number from PtCat/include/version.h")
    endif()
     set(var_patch ${CMAKE_MATCH_1})

    #将正则表达式获取到的数据保存到对应变量中
    #将访问权限放置到函数外部，我这里由于是 cmake 文件，在对应 cmakelists 文件中调用直接复制过去，所以不需要在函数外部再次 PARENT_SCOPE 调用一次了，当前函数这个作用域访问不到这个变量了
    set(PTCAT_VERSION_MAJOR ${var_major} PARENT_SCOPE)
    set(PTCAT_VERSION_MINOR ${var_minor} PARENT_SCOPE)
    set(PTCAT_VERSION_PATCH ${var_patch} PARENT_SCOPE)
    #设置一个最终版本的版本号
    set(PTCAT_VERSION "${var_major}.${var_minor}.${var_patch}" PARENT_SCOPE)
endfunction()

