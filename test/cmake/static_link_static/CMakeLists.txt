add_library(cmake_dep_2 STATIC dep_impl.cpp)

add_library(cmake_lib_2 STATIC impl.cpp)
target_link_libraries(cmake_lib_2 PUBLIC cmake_dep_2)
