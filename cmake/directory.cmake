# 添加指定目录下的所有子目录。仅添加包含 cmake 的子目录，非递归
function(poker_add_subdirectories)
    # 记录所有需要添加的子目录
    unset(subdirectories)

    # 如果传入的目录为空，则添加当前目录
    if ("${ARGN}" STREQUAL "")
        list(APPEND ARGN "./")
    endif ()

    # 依次查看给定目录
    foreach (dir ${ARGN})
        # 获取该目录下的所有文件
        file(GLOB subs ${dir}/*)

        # 若文件为目录并且该目录下存在 CMakeLists.txt，则添加该目录
        foreach (sub ${subs})
            if (IS_DIRECTORY ${sub} AND EXISTS "${sub}/CMakeLists.txt")
                list(APPEND subdirectories ${sub})
            endif ()
        endforeach ()
    endforeach ()

    # 添加所有目录
    foreach (subdir ${subdirectories})
        add_subdirectory(${subdir})
    endforeach ()

endfunction()