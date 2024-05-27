function(poker_add_target target_name target_type)
    include(GNUInstallDirs)

    # step 1: 添加本目标的源文件

    # 指定各类型目标的默认源文件位置
    set(target_type_for_src "executable" "library")

    if ("${target_type}" IN_LIST target_type_for_src)
        # 添加 src 目录下的同名目录
        if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/src/${target_name})
            list(APPEND config_SRC ${CMAKE_CURRENT_LIST_DIR}/src/${target_name})
        endif ()

    elseif ("${target_type}" STREQUAL "test")
        # 若为测试目标，则修改 target 名

        # 被测目标名
        set(target_tested_name ${target_name})
        # 测试目标默认源文件目录
        set(target_src_name ${target_name})
        # 测试目标名
        set(target_name ${target_name}-TEST)

        # 若指定 CASE 则，添加进目标名
        if (NOT "${config_CASE}" STREQUAL "")
            set(target_name ${target_name}-${config_CASE})
            set(target_src_name ${target_src_name}-${config_CASE})
        endif ()

        # 添加 test 目录下的同名目录
        if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/test/${target_src_name})
            list(APPEND config_SRC ${CMAKE_CURRENT_LIST_DIR}/test/${target_src_name})
        endif ()

    else ()
        message(FATAL_ERROR "undefined target type")

    endif ()

    foreach (src_dir ${config_SRC})
        if (EXISTS ${src_dir})
            file(GLOB_RECURSE target_sources ${src_dir}/*.cpp ${src_dir}/*.cc)
        endif ()
    endforeach ()

    # step 3: 创建目标

    # 判断是否为接口模式
    set(is_interface false)
    if ("${target_sources}" STREQUAL "")
        set(is_interface true)
    endif ()

    set(target_type_for_executable "executable" "test")
    if ("${target_type}" IN_LIST target_type_for_executable)
        # 构建可执行目标时没有提供源文件
        if (${is_interface})
            message(FATAL_ERROR "No source file is provided for executable target！")
        endif ()

        add_executable(${target_name} ${target_sources})

        # 若为测试目标，默认链接同名 target
        if (TARGET ${target_tested_name})
            target_link_libraries(${target_name} PRIVATE ${target_tested_name})
        endif ()

        list(APPEND poker_all_targets ${target_name})

    elseif ("${target_type}" STREQUAL "library")
        if (${is_interface})
            add_library(${target_name} INTERFACE)
        else ()
            if ("${config_SHARED}")
                add_library(${target_name} SHARED ${target_sources})
            else ()
                add_library(${target_name} ${target_sources})
            endif ()
        endif ()

        list(APPEND poker_all_targets ${target_name})

    else ()
        message(FATAL_ERROR "undefined target type")

    endif ()

    # step 4: 设置头文件搜索路径

    # 检查 INCLUDE 标签是否为绝对目录
    foreach (inc ${config_INCLUDE})
        cmake_path(IS_ABSOLUTE ${inc} is_absolute_inc)

        if (not "${is_absolute_inc}")
            message(FATAL_ERROR "Only absolute paths can be specified after INCLUDE！ Error path: ${inc}")
        endif ()
    endforeach ()

    # 添加当前处理的 List 文件下的 include 目录
    set(default_include_dir "include")
    if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/${default_include_dir}" AND NOT "${default_include_dir}" IN_LIST config_INC)
        list(APPEND config_INC "${default_include_dir}")
    endif ()

    if (${is_interface})
        # 接口模式中使用 INCLUDE 标签为错误用法
        if (NOT "${config_INCLUDE}" STREQUAL "")
            message(FATAL_ERROR "shouldn't use INCLUDE in INTERFACE target!")
        endif ()

        # INC 指定的该 target 自带的头文件位置
        foreach (inc_dir ${config_INC})
            list(APPEND interface_target_build_include "${CMAKE_CURRENT_LIST_DIR}/${inc_dir}")

            if ("${inc_dir}" STREQUAL include)
                list(APPEND interface_target_install_include "${CMAKE_INSTALL_INCLUDEDIR}")
            else ()
                list(APPEND interface_target_install_include "${CMAKE_INSTALL_INCLUDEDIR}/${inc_dir}")
            endif ()
        endforeach ()

        list(REMOVE_DUPLICATES interface_target_build_include)
        list(REMOVE_DUPLICATES interface_target_install_include)

        if (NOT ${interface_target_build_include} STREQUAL "")
            target_include_directories(${target_name} INTERFACE
                    "$<BUILD_INTERFACE:${interface_target_build_include}>"
                    "$<INSTALL_INTERFACE:${interface_target_install_include}>"
            )
        endif ()
    else ()
        # INC 指定的该 target 自带的头文件位置
        foreach (inc_dir ${config_INC})
            list(APPEND target_public_build_include "${CMAKE_CURRENT_LIST_DIR}/${inc_dir}")

            if ("${inc_dir}" STREQUAL include)
                list(APPEND target_public_install_include "${CMAKE_INSTALL_INCLUDEDIR}")
            else ()
                list(APPEND target_public_install_include "${CMAKE_INSTALL_INCLUDEDIR}/${inc_dir}")
            endif ()
        endforeach ()

        # INCLUDE 指定的该 target 依赖的头文件位置
        foreach (inc_dir ${config_INCLUDE})
            list(APPEND target_private_build_include "${inc_dir}")
        endforeach ()

        list(REMOVE_DUPLICATES target_private_build_include)
        list(REMOVE_DUPLICATES target_public_build_include)
        list(REMOVE_DUPLICATES target_public_install_include)

        if (NOT "${target_private_build_include}" STREQUAL "")
            target_include_directories(${target_name} PRIVATE
                    "$<BUILD_INTERFACE:${target_private_build_include}>"
            )
        endif ()

        if (NOT "${target_public_build_include}" STREQUAL "")
            target_include_directories(${target_name} PUBLIC
                    "$<BUILD_INTERFACE:${target_public_build_include}>"
                    "$<INSTALL_INTERFACE:${target_public_install_include}>"
            )
        endif ()
    endif ()

    # step 5: 添加依赖说明

    # 解析 DEPENDS、IMPORTS 后的 PRIVATE、INTERFACE 参数
    poker_split_arguments(config_DEPENDS ${config_DEPENDS})
    poker_split_arguments(config_IMPORTS ${config_IMPORTS})

    # step 5.1: 强制链接目标
    if (NOT "${config_FORCE_DEPENDS}" STREQUAL "")
        foreach (depend ${config_FORCE_DEPENDS})
            get_target_property(target_type "${depend}" TYPE)

            if (target_type STREQUAL STATIC_LIBRARY)
                list(APPEND target_link-force-STATIC ${depend})
            else ()
                list(APPEND target_link-force-SHARED ${depend})
            endif ()
        endforeach ()

        set(target_link-force
                "-Wl,--whole-archive " ${target_link-force-STATIC}" -Wl,--no-whole-archive"
                "-Wl,--no-as-needed " ${target_link-force-SHARED}" -Wl,--as-needed")
    endif ()

    # step 5.2: 外部依赖
    if (NOT "${config_IMPORTS_ALL}" STREQUAL "")
        poker_find_packages("${config_IMPORTS_ALL}" COMPONENTS "${config_IMPORTS_COMPONENTS}")

        # 通过变量导入外部依赖
        foreach (${package} ${config_IMPORTS_INTERFACE})
            list(APPEND target_include-INTERFACE ${package}_INCLUDE_DIRS)
            list(APPEND target_link-imported-INTERFACE ${package}_LIBRARIES)
        endforeach ()

        foreach (package ${config_IMPORTS_PRIVATE})
            list(APPEND target_include-PRIVATE ${package}_INCLUDE_DIRS)
            list(APPEND target_link-imported-PRIVATE ${package}_LIBRARIES)
        endforeach ()

        foreach (package ${config_IMPORTS_PUBLIC})
            list(APPEND target_include-PUBLIC ${package}_INCLUDE_DIRS)
            list(APPEND target_link-imported-PUBLIC ${package}_LIBRARIES)
        endforeach ()

        # 通过导入目标导入
        cmake_parse_arguments(imported_target "" "" "${config_IMPORTS_ALL}" ${config_IMPORTS_AS})

        # 仅可提供要求查找包的导入目标
        if (NOT "${imported_target_UNPARSED_ARGUMENTS}" STREQUAL "")
            message(FATAL_ERROR "Incorrect package was name provided while specifying imported！")
        endif ()

        foreach (package ${config_IMPORTS_ALL})
            if (NOT "${imported_target_${package}}" STREQUAL "")
                if (${package} IN_LIST config_IMPORTS_INTERFACE)
                    list(APPEND target_link-imported-INTERFACE ${imported_target_${package}})
                elseif (${package} IN_LIST config_IMPORTS_PRIVATE)
                    list(APPEND target_link-imported-PRIVATE ${imported_target_${package}})
                else ()
                    list(APPEND target_link-imported-PUBLIC ${imported_target_${package}})
                endif ()
            endif ()
        endforeach ()
    endif ()

    # step 5.3: 整合各个链接项，完成依赖传递
    set(target_link_interface_all
            ${config_DEPENDS_INTERFACE}
            ${target_link-imported-INTERFACE}
    )
    set(target_link_private_all
            ${target_link-force}
            ${config_DEPENDS_PRIVATE}
            ${target_link-imported-PRIVATE}
            ${config_LIBRARY}
    )
    set(target_link_public_all
            ${config_DEPENDS_PUBLIC}
            ${target_link-imported-PUBLIC}
            poker_compiler_flag
    )

    if (${is_interface})
        if (NOT "${target_link_private_all}" STREQUAL "")
            message(FATAL_ERROR " Interface targets cannot have private dependencies！ ")
        endif ()

        target_link_libraries(${target_name} INTERFACE
                ${target_link_public_all}
                ${target_link_interface_all}
        )
    else ()
        target_include_directories(${target_name} INTERFACE ${target_include-INTERFACE})
        target_include_directories(${target_name} PRIVATE ${target_include-PRIVATE})
        target_include_directories(${target_name} PUBLIC ${target_include-PUBLIC})

        target_link_libraries(${target_name} PRIVATE ${target_link_private_all})
        target_link_libraries(${target_name} INTERFACE ${target_link_interface_all})
        target_link_libraries(${target_name} PUBLIC ${target_link_public_all})
    endif ()

    # step 6: 安装该目标

    # 安装头文件
    foreach (inc ${config_INC})
        install(DIRECTORY ${inc}/
                DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
                FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.tpp"
        )
    endforeach ()

    # 安装二进制文件
    install(TARGETS ${target_name}
            EXPORT ${target_name}Target
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    # 安装导出文件
    install(EXPORT ${target_name}Target
            FILE "${target_name}Target.cmake"
            NAMESPACE ${PROJECT_NAME}::
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${target_name}"
    )

    # 生成包配置文件
    include(CMakePackageConfigHelpers)

    set(target_depends ${target_link_interface_all} ${target_link_private_all} ${target_link_public_all})

    configure_package_config_file(${poker_template_dir}/package_config.cmake
            "${CMAKE_CURRENT_BINARY_DIR}/${target_name}Config.cmake"
            INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${target_name}"
            NO_SET_AND_CHECK_MACRO
            NO_CHECK_REQUIRED_COMPONENTS_MACRO
    )

    write_basic_package_version_file(
            "${CMAKE_CURRENT_BINARY_DIR}/${target_name}ConfigVersion.cmake"
            VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}"
            COMPATIBILITY AnyNewerVersion
    )

    # 安装包配置文件
    install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/${target_name}Config.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/${target_name}ConfigVersion.cmake
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${target_name}"
    )
endfunction()