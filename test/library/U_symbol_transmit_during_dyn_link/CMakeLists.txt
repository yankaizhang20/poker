add_library(dynlib SHARED library_a.cpp library_b.cpp)

# note: 若想查看测试结果，需要取消下面的注释
#target_compile_definitions(dynlib PUBLIC TEST)

#结论: 组成动态库的所有编译单元的未定义的符号都会传递到可执行文件中
add_executable(U_symbol_transmit_during_dyn_link main.cpp)
target_link_libraries(U_symbol_transmit_during_dyn_link dynlib)