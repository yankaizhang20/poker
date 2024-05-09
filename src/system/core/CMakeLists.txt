###################################################
# 框架核心
###################################################

add_library(core src/core/core.cpp)
target_link_libraries(core PUBLIC poker_compiler_flag system-launch_center)
target_link_libraries(core PUBLIC system-auto_start)
target_include_directories(core PUBLIC include)

add_executable(core-TEST test/main.cpp)
target_link_libraries(core-TEST PRIVATE core)