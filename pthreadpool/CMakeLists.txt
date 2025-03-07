#[[
在 CLion 中，即使在 CMake 中正确指定了头文件路径，但对应的 `.cpp` 文件仍然报错找不到头文件，可能是由以下原因导致的：
### 1. **头文件路径未正确传递**
- 确保在 `CMakeLists.txt` 文件中正确使用了 `target_include_directories` 或 `include_directories` 来指定头文件路径。
- 如果使用 `target_include_directories`，需要明确指定作用范围（如 `PRIVATE`、`PUBLIC` 或 `INTERFACE`），例如：
  ```cmake
  target_include_directories(my_target PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
  ```
  如果路径未正确传递，可能会导致编译器无法找到头文件。

### 2. **路径拼接问题**
- 如果头文件路径是相对路径，确保路径拼接正确。例如，如果头文件路径是 `/path/to/include`，而代码中使用的是 `#include "subdir/header.h"`，则需要确保 `include` 目录的父路径被正确添加。

### 3. **CLion 缓存问题**
- CLion 的缓存可能会导致头文件路径未被正确识别。尝试以下操作：
  - 清理 CLion 缓存：`File -> Invalidate Caches / Restart`。
  - 重新加载 CMake 项目：`Tools -> CMake -> Reload CMake Project`。

### 4. **头文件路径未被识别**
- 如果头文件路径是通过变量传递的，确保变量被正确设置。例如：
  ```cmake
  set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
  target_include_directories(my_target PRIVATE ${INCLUDE_DIRS})
  ```
- 如果路径未被正确识别，可以尝试直接使用绝对路径。

### 5. **头文件路径未被正确解析**
- 如果头文件路径是通过 `target_include_directories` 指定的，确保路径的可见性（`PRIVATE`、`PUBLIC` 或 `INTERFACE`）与目标的使用场景一致。

### 6. **CLion 索引问题**
- CLion 的索引器可能会导致某些头文件无法被识别。可以尝试以下操作：
  - 关闭项目，删除 `.idea` 文件夹，然后重新打开项目。
  - 确保头文件路径被正确添加到 `CMakeLists.txt` 中，并且路径是正确的。

### 7. **远程开发问题**
- 如果使用远程开发，确保远程主机上的头文件路径与本地一致，并且通过 `Tools -> Resync with Remote Hosts` 同步。

如果以上方法仍未解决问题，可以尝试以下步骤：
- 检查头文件是否确实存在于指定路径中。
- 确保头文件的包含方式正确（如使用双引号 `"` 或尖括号 `<>`）。
- 如果问题仍然存在，可以尝试更新 CLion 或 CMake 的版本。
]]
cmake_minimum_required(VERSION 3.28)
project(PThreadPool)

#message(STATUS "** pthreadpool project name : ${PROJECT_NAME}")#指定下等级，--log-level=VERBOSE 可以看到


#设置当前源文件地址
set(TPOOL_CPP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
#message(STATUS "** TPOOL_CPP_DIR : ${TPOOL_CPP_DIR}")

#尝试获取下头文件地址
#message(STATUS "** TPOOL_H_DIR : ${TPOOL_H_DIR}")

#include_directories(TPOOL_H_DIR)#头文件引用带有全局性质，如果其含有子类，可能会有问题，使用下面的 target_include_directories来为当前 cmake 和其依赖者设置头文件路径
#获取到某一个路径下的所有源文件
aux_source_directory(${TPOOL_CPP_DIR} SOURCE_FILES)
#message(STATUS "** SOURCE_FILES : ${SOURCE_FILES}")

#获取到对应路径下的所有的头文件
#file(GLOB HEARD_FILES "${TPOOL_H_DIR}/*.h*")
#message(STATUS "** HEARD_FILES : ${HEARD_FILES}")

#创建一个缓存变量
option(TPOOL_SHARED "create shared lib or static lib" ON)#注意这里如果是设置为了动态库，需要对应头文件加 dllexport, 否则生成不了 .lib 函数地址文件
#message(STATUS "** TPOOL_SHARED : ${TPOOL_SHARED}")

#设置是生成动态库还是静态库 ON 是生成动态库， OFF 是生成静态库
set(BUILD_SHARED_LIBS ${TPOOL_SHARED})
add_library(${PROJECT_NAME} ${SOURCE_FILES}
        include/ptask.h)#如果头文件和子文件不在一个文件夹中，设置头文件好像会出错，这里尽量别设置头文件

#判断一下，如果是静态库，那么需要穿递给 C++ 一个宏，告诉他这个是静态库
if (NOT TPOOL_SHARED)
    #这里写在下面是因为想指定对应传宏的目标，我这里只针对当前库以及其依赖者 add_definitions 也可以，但是其是全局的，比较老
    target_compile_definitions(${PROJECT_NAME} PUBLIC "STATIC_LIB")
endif ()

#[[
    为对应目标以及其依赖者设置头文件地址，例如 pthreadpool.cpp 就是其依赖者
        add_library 设置目标以及这个目标的依赖者，所以需要 target_include_directories 要写在 add_library 的后面
        同理 target_link_directories 也一样
]]
target_include_directories(${PROJECT_NAME} PUBLIC ${TPOOL_H_DIR})


