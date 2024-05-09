# 线程安全栈

add_library(thread-stack src/thread-stack/stack.cpp)
target_link_libraries(thread-stack PUBLIC poker_compiler_flag)
target_include_directories(thread-stack PUBLIC include)