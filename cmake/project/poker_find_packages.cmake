###################################################################################
# eon_find_packages(
#                       <name ...>
#                       [AS <result>]
#                       [COMPONENTS [<name> <name-component> ...]]
#                  )
###################################################################################
# 查找指定多个第三库，并将头文件目录、库文件路径添加到对应或指定变量中。
#
# 可传入多个 name 参数，代表查找多个第三方库，并将头文件目录写入到 ${name}_INCLUDE_DIRS、
# 变量中，将库文件路径写入到 ${name}_LIBRARIES 变量中。
###################################################################################
# AS：将找到的所有头文件目录、库文件路径，记录到 ${result}_INCLUDE_DIRS、${result}_LIBRARIES变量中。
#
# COMPONENTS：各个待导入的第三方库的组件，使用各个第三方库的名称进行分组。
###################################################################################
# @example
# eon_find_packages(
#   eigen3 Boost
#   COMPONENTS
#       Boost system thread
#   AS result
# )
# message(${Boost_INCLUDE_DIRS} ${result_LIBRARIES})
###################################################################################

macro(poker_find_packages)
    # 解析位置参数
    cmake_parse_arguments(config "" "AS" "COMPONENTS" ${ARGS})

    # 要求查找的所有包
    set(poker_find_packages-packages ${config_UNPARSED_ARGUMENTS})

    if ("${poker_find_packages-packages}" STREQUAL "")
        message(FATAL_ERROR "No package provided！")
    endif ()


    # 划分各包的组件
    cmake_parse_arguments(components "" "" ${poker_find_packages-packages} ${config_COMPONENTS})

    # 仅可查找要求查找包的组件
    if (NOT "${components_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(FATAL_ERROR "Incorrect package name provided！")
    endif ()

    # 依次查找所有包
    foreach (package ${poker_find_packages-packages})
        if (${components_${package}} STREQUAL "")
            find_package(${package} REQUIRED)
        else ()
            find_package(${package} REQUIRED COMPONENTS ${components_${package}})
        endif ()
    endforeach ()

    # 若找所有到包并，按要求汇总结果
    if (NOT "${config_AS}" STREQUAL "" AND ${package}_FOUND)
        foreach (package ${poker_find_packages-packages})
            set(${config_AS}_INCLUDE_DIR ${package}_INCLUDE_DIR)
            set(${config_AS}_LIBRARY ${package}_LIBRARY)

            list(APPEND ${config_AS}_INCLUDE_DIRS ${config_AS}_INCLUDE_DIR)
            list(APPEND ${config_AS}_LIBRARIES ${config_AS}_LIBRARY)
        endforeach ()
    endif ()
endmacro()