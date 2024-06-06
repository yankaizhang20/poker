cmake_minimum_required(VERSION 3.15)

# 项目信息
project(poker
        VERSION 0.0.1
        DESCRIPTION "c++ 通用工具箱"
        LANGUAGES CXX
)

# 引入 cmake 工具
include(cmake/main.cmake)

poker_project()

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_LIST_DIR}/poker_install)

# 第三方库
poker_add_subdirectories(3rd)

# 源代码
poker_add_subdirectories(src)

# 实验
poker_add_subdirectories(test)

poker_finish_project()