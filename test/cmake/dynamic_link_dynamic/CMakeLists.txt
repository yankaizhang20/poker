add_library(cmake_dep_3 SHARED dep_impl.cpp)

add_library(cmake_lib_3 SHARED impl.cpp)
target_link_libraries(cmake_lib_3 PUBLIC cmake_dep_3)
