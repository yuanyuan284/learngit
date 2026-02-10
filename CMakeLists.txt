cmake_minimum_required(VERSION 3.10)
project(library_queue_system C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 添加UTF-8支持
if(MSVC)
    add_compile_options(/utf-8)
else()
    add_compile_options(-finput-charset=UTF-8 -exec-charset=GBK)
endif()

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 包含头文件目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 设置源码文件
set(SOURCES
    src/main.c
    src/reader.c
    src/queue.c
    src/service.c
    src/window.c
    src/statistics.c
    src/persistence.c
)

# 添加可执行文件
add_executable(library_queue_system ${SOURCES})

# Windows特定设置
if(WIN32)
    target_compile_definitions(library_queue_system PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# 设置调试和发布版本的不同编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(library_queue_system PRIVATE -g -O0 -Wall -Wextra)
else()
    target_compile_options(library_queue_system PRIVATE -O2 -DNDEBUG)
endif()