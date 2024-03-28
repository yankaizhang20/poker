cmake_minimum_required(VERSION 3.15)
project(poker
        VERSION 0.0.1
        DESCRIPTION "c++ 通用工具箱"
        LANGUAGES CXX
)

# 设置 c++ 标准，以及常用的编译、链接选项
add_library(poker_compiler_flag INTERFACE)
target_compile_features(poker_compiler_flag INTERFACE cxx_std_17)
target_link_options(poker_compiler_flag INTERFACE "-pthread")

add_subdirectory(thread)
add_subdirectory(concurrency)
add_subdirectory(test)