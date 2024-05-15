# 结论： 有依赖关系 a->b->c ,当 b private link c 时，c 依然会被链接入 a
add_executable(target_link_libraries-private-a main.cpp)
add_library(target_link_libraries-private-b lib.cpp)
add_library(target_link_libraries-private-c libdep.cpp)

target_link_libraries(target_link_libraries-private-b PRIVATE target_link_libraries-private-c)

target_link_libraries(target_link_libraries-private-a PRIVATE target_link_libraries-private-b)