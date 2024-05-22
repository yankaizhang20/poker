function(poker_project)
    # 记录构建的所有 target
    set(poker_all_targets "" CACHE STRING "all targets in build tree")

    # 设置 c++ 标准，以及常用的编译、链接选项
    poker_add_library(poker_compiler_flag)

    target_compile_features(poker_compiler_flag INTERFACE cxx_std_17)
    target_link_options(poker_compiler_flag INTERFACE "-pthread")
endfunction()