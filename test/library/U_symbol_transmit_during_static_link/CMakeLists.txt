add_library(staticlib STATIC library.cpp)

# note：若查看看测试结果，需取消下面一行的注释
#target_compile_definitions(staticlib PUBLIC TEST)

#结论：静态库中未定义的符号按编译单元传递
add_executable(U_symbol_transmit_during_static_link main.cpp)
target_link_libraries(U_symbol_transmit_during_static_link PRIVATE staticlib)