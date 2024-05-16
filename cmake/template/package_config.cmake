###################################################################################
# 组件配置文件
###################################################################################
# 本模板使用以下变量：

# project_name: 工程名称

# target_name: target 名称

# target_inner_depends: 所依赖的本工程内的 target

# target_outer_depends: 外部依赖

# target_file_name: 包含导出 target 文件的名称
###################################################################################

#@PACKAGE_INIT@

# step 1: 导入该 target 的所有依赖
foreach (inner_depends @target_inner_depends@)
    find_dependency(${inner_depends})
endforeach ()

foreach (outer_depends @target_outer_depends@)
    find_dependency(${outer_depends})
endforeach ()

# step 2: 包含导入文件
include("${CMAKE_CURRENT_LIST_DIR}/@target_file_name@.cmake")
