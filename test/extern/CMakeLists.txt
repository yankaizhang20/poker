add_library(mylib SHARED lib.cpp)
add_executable(myexe main.cpp)
target_link_libraries(myexe mylib)