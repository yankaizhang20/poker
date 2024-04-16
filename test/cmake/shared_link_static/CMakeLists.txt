set(CMAKE_VERBOSE_MAKEFILE ON)

add_library(cmake_staticlib STATIC static_impl.cpp)

add_library(cmake_dynamiclib SHARED dynamic_impl.cpp)
target_link_libraries(cmake_dynamiclib PUBLIC cmake_staticlib)
