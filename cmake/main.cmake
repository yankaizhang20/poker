# 添加指定目录下的所有 cmake 文件
function(collect_tools_of path)
    file(GLOB_RECURSE cmake_files ${path} "${path}/*.cmake")

    foreach (cmake_file ${cmake_files})
        include(${cmake_file})
    endforeach ()
endfunction()

# 收集所有功能
collect_tools_of(${CMAKE_CURRENT_LIST_DIR}/project)
collect_tools_of(${CMAKE_CURRENT_LIST_DIR}/target)

# 模板目录
set(poker_template_dir ${CMAKE_CURRENT_LIST_DIR}/template)

# 脚本目录
set(poker_script_dir ${CMAKE_CURRENT_LIST_DIR}/script)
