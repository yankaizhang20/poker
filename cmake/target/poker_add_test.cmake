##################################################################################################################################
# poker_add_test( <target>
#                       [CASE                   case_name        ]
#                       [INC                    dir ... ]
#                       [SRC                    dir ... ]
#                       [DEPENDS                target ... [PRIVATE target ...] [INTERFACE target ...] ]
#                       [FORCE_DEPENDS          target ... ]
#                       [IMPORTS                package ... [PRIVATE package ...] [INTERFACE package ...] ]
#                       [IMPORTS_COMPONENTS     <package component ...> ... ]
#                       [IMPORTS_AS             <package imported-target ...> ... ]
#                       [INCLUDE                dir ... ]
#                       [LIBRARY                library ... ]
#                     )
##################################################################################################################################
#
# 构建可执行 target，同时指定其对应的头文件、源文件目录以及相应的依赖。
#
##################################################################################################################################
#
# CASE: 测试用例名，将添加到 target 名后
#
# INC: 指定本 target 提供（创建）的头文件所在目录，也将安装该目录中的头文件。默认包含当前处理 list 文件下的 include 目录。
#
# SRC: 指定构建本 target 所需源文件的目录。默认包含当前处理 list 文件下名为 target 的目录。若没有任何源文件提供，将构建为 interface target
#
# DEPENDS: 依赖本工程内的其他 target，默认将传递该依赖。通过指定 PRIVATE 阻止依赖传递
#
# FORCE_DEPENDS: 强制链接本工程内的其他 target。不继承其使用说明
#
# IMPORTS: 依赖系统中的外部库，默认将传递该依赖。通过指定 PRIVATE 组织依赖传递
#
# IMPORTS_COMPONENTS: 指定依赖外部库的组件，使用各个第三方库的名称进行分组
#
# IMPORTS_AS: 指定外部库的导入目标
#
# LIBRARY: 直接依赖系统中的外部库
#
# INCLUDE: 指定头文件搜索目录，仅可使用绝对目录。仅作为私有依赖
#
##################################################################################################################################

function(poker_add_test target_name)
    poker_parse_arguments(config "" "CASE" "" ${ARGN})

    poker_add_target(${target_name} test)
endfunction()