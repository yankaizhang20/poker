# 在这里给出配置中的各个标签
macro(poker_parse_arguments prefix options one_value_keywords multi_value_keywords)
    # 设置位置参数
    list(APPEND options_ ${options})

    list(APPEND one_value_keywords_ ${one_value_keywords})

    list(APPEND multi_value_keywords_
            ${multi_value_keywords}
            INC
            SRC
            DEPENDS
            FORCE_DEPENDS
            IMPORTS
            IMPORTS_COMPONENTS
            IMPORTS_AS
            INCLUDE
            LIBRARY
    )

    # 去除重复
    list(REMOVE_DUPLICATES options_)
    list(REMOVE_DUPLICATES one_value_keywords_)
    list(REMOVE_DUPLICATES multi_value_keywords_)

    # 解析位置参数
    cmake_parse_arguments("${prefix}" "${options_}" "${one_value_keywords_}" "${multi_value_keywords_}" ${ARGN})

    # 若有未知的参数，给出提示
    if (NOT "${${prefix}_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(FATAL_ERROR "unknown arguments: ${${prefix}_UNPARSED_ARGUMENTS}")
    endif ()
endmacro()

# 区分参数中的 PUBLIC PRIVATE INTERFACE 参数，并提供囊括所有类型参数
macro(poker_split_arguments prefix)
    cmake_parse_arguments(${prefix} "" "" "PRIVATE;INTERFACE" ${ARGN})

    set(${prefix}_PUBLIC "${${prefix}_UNPARSED_ARGUMENTS}")

    set(${prefix}_ALL ${${prefix}_PUBLIC} ${${prefix}_PRIVATE} ${${prefix}_INTERFACE})
endmacro()