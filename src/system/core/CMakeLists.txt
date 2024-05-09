###################################################
# 框架核心
###################################################

add_library(core src/core/core.cpp)
target_link_libraries(core PUBLIC poker_compiler_flag system-launch_center)
target_link_libraries(core PUBLIC system-auto_start)
target_include_directories(core PUBLIC include)

add_executable(core-LAUNCH test/core-LAUNCH/main.cpp)
target_link_libraries(core-LAUNCH PRIVATE core)

add_executable(core-PROCESS test/core-PROCESS/main.cpp)
target_link_libraries(core-PROCESS PRIVATE core)
