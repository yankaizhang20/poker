add_library(cmake_dynamiclib_1 SHARED dynamic_impl.cpp)

add_library(cmake_staticlib_1 STATIC static_impl.cpp)
target_link_libraries(cmake_staticlib_1 PUBLIC cmake_dynamiclib_1)
