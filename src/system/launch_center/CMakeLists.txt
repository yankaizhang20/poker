add_library(system-launch_center src/system-launch_center/launch_center.cpp)
target_link_libraries(system-launch_center PUBLIC poker_compiler_flag pattern)
target_include_directories(system-launch_center PUBLIC include)

add_executable(system-launch_center-TEST test/main.cpp)
target_link_libraries(system-launch_center-TEST PRIVATE system-launch_center)