###################################################
# 框架核心
###################################################

poker_add_library(core DEPENDS system-launch_center system-auto_start)

add_executable(core-LAUNCH test/core-LAUNCH/main.cpp)
target_link_libraries(core-LAUNCH PRIVATE core)

add_executable(core-PROCESS test/core-PROCESS/main.cpp)
target_link_libraries(core-PROCESS PRIVATE core)
