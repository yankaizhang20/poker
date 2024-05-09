# 线程安全队列

add_library(thread-queue src/thread-queue/queue.cpp)
target_link_libraries(thread-queue PUBLIC poker_compiler_flag)
target_include_directories(thread-queue PUBLIC include)