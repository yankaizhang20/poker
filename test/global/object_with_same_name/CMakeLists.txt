add_library(object_with_same_name-var_a SHARED var_a.cpp)

add_library(object_with_same_name-var_b STATIC var_b.cpp)

# 如此，会 double free
add_executable(object_with_same_name main.cpp)
target_link_libraries(object_with_same_name PUBLIC
        object_with_same_name-var_a
        -Wl,--whole-archive object_with_same_name-var_b -Wl,--no-whole-archive
)