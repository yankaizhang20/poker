poker_add_library(macro)

add_executable(macro-TEST test/main.cpp)
target_link_libraries(macro-TEST PRIVATE macro)