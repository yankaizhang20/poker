###################################################
# 自启动模块
###################################################

add_library(system-auto_start src/system-auto_start/AutoComponent.cpp)
target_link_libraries(system-auto_start PRIVATE system-launch_center)
target_link_libraries(system-auto_start PUBLIC macro)
target_include_directories(system-auto_start PUBLIC include)