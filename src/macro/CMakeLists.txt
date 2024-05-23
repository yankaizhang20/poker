poker_add_library(macro)

poker_add_executable(macro-test-poker_add_executable SRC test DEPENDS macro)

add_executable(macro-TEST test/main.cpp)
target_link_libraries(macro-TEST PRIVATE macro)