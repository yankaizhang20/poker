add_library(thread-manage INTERFACE)
target_include_directories(thread-manage INTERFACE include)

add_executable(thread-manage_test test/main.cpp)
target_link_libraries(thread-manage_test
                      PUBLIC poker_compiler_flag
                             thread-manage
)
