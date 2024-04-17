add_library(cmake_dep_static STATIC src/dep_impl.cpp)
add_library(cmake_dep_dynamic SHARED src/dep_impl.cpp)

add_library(cmake_static_link_dynamic STATIC src/impl.cpp)
target_link_libraries(cmake_static_link_dynamic PUBLIC cmake_dep_dynamic)

add_library(cmake_dynamic_link_static SHARED src/impl.cpp)
target_link_libraries(cmake_dynamic_link_static PUBLIC cmake_dep_static)

add_library(cmake_static_link_static STATIC src/impl.cpp)
target_link_libraries(cmake_static_link_static PUBLIC cmake_dep_static)

add_library(cmake_dynamic_link_dynamic SHARED src/impl.cpp)
target_link_libraries(cmake_dynamic_link_dynamic PUBLIC cmake_dep_dynamic)