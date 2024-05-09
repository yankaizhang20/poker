add_library(pattern INTERFACE)
target_include_directories(pattern INTERFACE include)

add_executable(pattern-TEST test/main.cpp)
target_link_libraries(pattern-TEST PRIVATE poker_compiler_flag pattern)