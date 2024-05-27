###################################################################################
#
# 组件配置文件
#
###################################################################################
#
# 本模板使用以下变量：
#
# target_name: target 名称
#
# target_inner_depends: 所依赖的本工程内的 target
#
# target_outer_depends: 外部依赖
#
###################################################################################

#@PACKAGE_INIT@

# step 1: 导入该 target 的所有依赖

include(CMakeFindDependencyMacro)

foreach (depend @target_depends@)
    find_dependency(${depend})
endforeach ()

# step 2: 包含导入文件
include("${CMAKE_CURRENT_LIST_DIR}/@target_name@Target.cmake")
