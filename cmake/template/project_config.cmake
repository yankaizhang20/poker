###################################################################################
# 顶层包配置文件
###################################################################################
# 本模板使用以下变量：

# project_name: 工程名称

# poker_all_targets: 构建的所有 target
###################################################################################

#@PACKAGE_INIT@

# step 1: 检查用户输入的组件是否全部存在
foreach (comp ${@project_name@_FIND_COMPONENTS})
    if (NOT comp IN_LIST @poker_all_targets@)
        set(@project_name@_FOUND False)
        set(@project_name@_NOT_FOUND_MESSAGE "@project_name@ unsupported component: ${comp}")

        return()
    endif ()
endforeach ()


# step 2: 导入用户需要的组件

include(CMakeFindDependencyMacro)

# 保存原有的 CMAKE_PREFIX_PATH
set(CMAKE_PREFIX_PATH_old ${CMAKE_PREFIX_PATH})
set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR}/components)

foreach (comp ${@project_name@_FIND_COMPONENTS})
    find_dependency(${comp} REQUIRED)
endforeach ()

# 恢复原有的 CMAKE_PREFIX_PATH
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH_old})




