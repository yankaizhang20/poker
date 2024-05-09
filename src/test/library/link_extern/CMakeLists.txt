add_library(extern_impl STATIC impl.cpp)

add_library(user_interface STATIC user.cpp)
target_link_libraries(user_interface PUBLIC extern_impl)

add_executable(link_extern main.cpp)
target_link_libraries(link_extern user_interface)