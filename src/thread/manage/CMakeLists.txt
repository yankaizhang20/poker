poker_add_library(thread-manage DEPENDS thread-lock-hierarchical)

add_executable(thread-manage_test test/main.cpp)
target_link_libraries(thread-manage_test
        PUBLIC poker_compiler_flag
        thread-manage
)


install(TARGETS thread-manage_test DESTINATION bin)