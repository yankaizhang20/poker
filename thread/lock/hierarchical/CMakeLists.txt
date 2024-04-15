add_library(thread-lock INTERFACE)
target_include_directories(thread-lock INTERFACE include)

# TODO: 换成使用 target_sources() 安装头文件目录
install(FILES include/hierarchical_mutex.h DESTINATION include)