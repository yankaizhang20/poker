add_library(macro INTERFACE)
target_include_directories(macro INTERFACE include)

add_executable(macro-TEST test/main.cpp)
target_link_libraries(macro-TEST PRIVATE macro)