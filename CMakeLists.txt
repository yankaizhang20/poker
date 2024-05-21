cmake_minimum_required(VERSION 3.15)

# TODO: 实现导出后 target 依赖可以在 find_package(target) 中自动导入

# 项目信息
project(poker
        VERSION 0.0.1
        DESCRIPTION "c++ 通用工具箱"
        LANGUAGES CXX
)

# 引入 cmake 工具
include(cmake/main.cmake)

# 设置 c++ 标准，以及常用的编译、链接选项
add_library(poker_compiler_flag INTERFACE)
target_compile_features(poker_compiler_flag INTERFACE cxx_std_17)
target_link_options(poker_compiler_flag INTERFACE "-pthread")

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_LIST_DIR}/poker_install)

# 源代码
poker_add_subdirectories(src)

# 实验
poker_add_subdirectories(test)


poker_finish_project()