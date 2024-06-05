##################################################################################################################################
# poker_add_protobuf( <target>
#                       [SHARED]
#                       [PROTO           dir ...     ]
#                       [DEPENDS         target ...  ]
#                     )
##################################################################################################################################
#
# 将 proto 文件构建为库，头文件将保持原有目录结构
#
##################################################################################################################################
#
# SHARED: 将 proto 构建为动态库
#
# PROTO: 指定构建本 target 所需 proto 文件所在的目录。若没有指定，将默认包含当前处理 list 文件下名为 proto 的目录
#
# DEPENDS: 依赖本工程内的其他 proto target
#
##################################################################################################################################

function(poker_add_protobuf target_name)
    # step 1 : 识别位置参数
    cmake_parse_arguments(config "SHARED" "" "PROTO;DEPENDS")

    # 若没有指定 proto 文件目录，将默认包含当前 List 文件下 proto 目录下的同名 target 目录
    if ("${config_PROTO}" STREQUAL "")
        set(config_PROTO ${CMAKE_CURRENT_LIST_DIR}/proto/${target_name})
    endif ()

    # step 2 : 编译指定的 proto 文件

    # 将编译后的文件放在 .poker 目录下
    set(poker_protobuf_root ${CMAKE_BINARY_DIR}/.poker/proto/${target_name})

    foreach (path ${config_PROTO})
        # 收集 proto 目录
        set(proto_paths "${proto_paths} -I=${path}")

        # 收集 proto 文件
        file(GLOB_RECURSE files FOLLOW_SYMLINKS ${path}/*.proto)
        foreach (file ${files})
            set(proto_files "${proto_files} ${file}")
        endforeach ()
    endforeach ()

    # 编译 proto 文件
    set(generate_by_protoc ${poker_script_dir}/generate_protobuf_package.bash)
    execute_process(
            COMMAND ${generate_by_protoc} ${target_name} ${poker_protobuf_root} ${proto_paths} ${proto_files}
            OUTPUT_VARIABLE result
            ERROR_VARIABLE error
            RESULT_VARIABLE code
    )

    # step 3 : 生成库目标

    if("${config_SHARED}")
        set(is_shared "SHARED")
    endif ()

    configure_file(
            "${poker_template_dir}/protobuf_target_CMakeLists.txt"
            "${poker_protobuf_root}/CMakeLists.txt"
            @ONLY
    )

    add_subdirectory("${poker_protobuf_root}" "${poker_protobuf_root}.output")

endfunction()

