cmake_minimum_required(VERSION 3.15)

# TODO: 实现导出后 target 依赖可以在 find_package(target) 中自动导入

# 项目信息
project(poker
        VERSION 0.0.1
        DESCRIPTION "c++ 通用工具箱"
        LANGUAGES CXX
)

# 设置 c++ 标准，以及常用的编译、链接选项
add_library(poker_compiler_flag INTERFACE)
target_compile_features(poker_compiler_flag INTERFACE cxx_std_17)
target_link_options(poker_compiler_flag INTERFACE "-pthread")

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_LIST_DIR}/poker_install)

# 组件
add_subdirectory(thread)
add_subdirectory(concurrency)
add_subdirectory(test)

# distribute
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "POKER")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "POKER IS A BASE C++ LIBRARY")
set(CPACK_DEBIAN_PACKAGE_VERSION_MAJOR "${poker_VERSION_MAJOR}")
set(CPACK_DEBIAN_PACKAGE_VERSION_MINOR "${poker_VERSION_MINOR}")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6), libc6 (<< 3)")
include(CPack)

