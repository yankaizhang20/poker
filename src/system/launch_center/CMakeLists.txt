poker_add_library(system-launch_center DEPENDS pattern)

add_executable(system-launch_center-TEST test/main.cpp)
target_link_libraries(system-launch_center-TEST PRIVATE system-launch_center)