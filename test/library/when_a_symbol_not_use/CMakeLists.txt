#add_library(staticlib STATIC library.cpp)
add_library(dynamiclib SHARED library.cpp)

#结论：executable target 中的所有符号都是需要被解析到的
add_executable(when_a_symbol_not_use main.cpp)
target_link_libraries(when_a_symbol_not_use dynamiclib)