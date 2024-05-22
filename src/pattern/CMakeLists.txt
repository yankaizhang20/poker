poker_add_library(pattern)

add_executable(pattern-TEST test/main.cpp)
target_link_libraries(pattern-TEST PRIVATE poker_compiler_flag pattern)