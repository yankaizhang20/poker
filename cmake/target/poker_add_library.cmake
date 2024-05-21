###################################################################################
# poker_add_library( <target>
#                    [SHARED]
#                    [DEPENDS                target ... [PRIVATE target ...] [INTERFACE target ...] ]
#                    [IMPORTS                package ... [PRIVATE package ...] [INTERFACE package ...] ]
#                    [IMPORTS_COMPONENTS     <package component ...> ... ]
#                    [IMPORTS_AS             <package|component imported-target> ... ]
#                    [INCLUDE                <dir ... [PRIVATE dir ...] [INTERFACE dir ...]> ]
#                    [FORCE_DEPENDS          target ... ]
#                    [LIBRARY                library ... ]
#                    [  SRC                  dir ... ]
#                  )
###################################################################################
# 头文件:  1.将 include 作为头文件默认搜索路径，并作为本 target 的使用要求，依赖本 target 的
#          其他 target 将看到该目录内的头文件
#
# 源文件:  1.添加 src 目录下名为 target 目录下的全部源文件
#         2.添加 SRC 指定的所有目录下的源文件
#         3.对以上目录递归搜索
#         4.若没有源文件，则作为 interface target
###################################################################################
# SHARED: 指定构建为动态库
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
# INCLUDE: 指定头文件搜索目录，默认传递该目录。通过指定 PRIVATE 组织依赖传递 TODO: 导出目录在生成表达式中应为相对目录？
#
# SRC: 额外添加源文件目录
###################################################################################

function(poker_add_library target_name)
    include(GNUInstallDirs)

    # step 1: 解析位置参数
    poker_parse_arguments(config "SHARED" "" "" ${ARGN})

    # step 2: 添加本目标的源文件
    set(poker_target_sources)

    # 查看 src 目录下的同名目录、SRC 指定的目录
    if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/src/${target_name})
        list(APPEND config_SRC ${CMAKE_CURRENT_LIST_DIR}/src/${target_name})
    endif ()

    if (EXISTS ${config_SRC})
        foreach (src_dir ${config_SRC})
            file(GLOB_RECURSE poker_target_sources "${src_dir}/*.cpp;${src_dir}/*.cc")
        endforeach ()
    endif ()

    # step 3: 创建目标

    # 判断是否为接口模式
    set(is_interface false)
    if ("${poker_target_sources}" STREQUAL "")
        set(is_interface true)
    endif ()


    # 创建目标
    list(APPEND poker_all_targets ${target_name})

    if (${is_interface})
        add_library(${target_name} INTERFACE)
    else ()
        if ("${config_SHARED}" STREQUAL "")
            add_library(${target_name} SHARED ${poker_target_sources})
        else ()
            add_library(${target_name} ${poker_target_sources})
        endif ()
    endif ()

    # step 4: 设置头文件搜索路径

    # 解析 INCLUDE 后的 PRIVATE、INTERFACE参数
    poker_split_arguments(config_INCLUDE ${config_INCLUDE})

    if (${is_interface})
        # 接口模式中使用 INCLUDE PRIVATE 为错误用法
        if (NOT "${config_INCLUDE_Private}" STREQUAL "")
            message(FATAL_ERROR "shouldn't use INCLUDE with PRIVATE in INTERFACE target!")
        endif ()

        target_include_directories(${target_name} INTERFACE
                $<BUILD_INTERFACE:
                ${CMAKE_CURRENT_LIST_DIR}/include>,
                ${CMAKE_CURRENT_LIST_DIR}/${config_INCLUDE_Public},
                ${CMAKE_CURRENT_LIST_DIR}/${config_INCLUDE_INTERFACE}
                $<INSTALL_INTERFACE:
                ${CMAKE_INSTALL_INCLUDEDIR},
                ${CMAKE_INSTALL_INCLUDEDIR}/${config_INCLUDE_Public},
                ${CMAKE_INSTALL_INCLUDEDIR}/${config_INCLUDE_INTERFACE}>
        )
    else ()
        target_link_libraries(${target_name} PRIVATE
                $<BUILD_INTERFACE: ${CMAKE_CURRENT_LIST_DIR}/${config_INCLUDE_Private}>
        )

        target_include_directories(${target_name} INTERFACE
                $<BUILD_INTERFACE:
                ${CMAKE_CURRENT_LIST_DIR}/${config_INCLUDE_INTERFACE}>
                $<INSTALL_INTERFACE:
                ${CMAKE_INSTALL_INCLUDEDIR}/${config_INCLUDE_INTERFACE}>
        )

        target_include_directories(${target_name} PUBLIC
                $<BUILD_INTERFACE:
                ${CMAKE_CURRENT_LIST_DIR}/include,
                ${CMAKE_CURRENT_LIST_DIR}/${config_INCLUDE_Public}>
                $<INSTALL_INTERFACE:
                ${CMAKE_INSTALL_INCLUDEDIR},
                ${CMAKE_INSTALL_INCLUDEDIR}/${config_INCLUDE_Public}>
        )
    endif ()

    # step 5: 添加依赖说明

    # 解析 DEPENDS、IMPORTS 后的 PRIVATE、INTERFACE 参数
    poker_split_arguments(config_DEPENDS ${config_DEPENDS})
    poker_split_arguments(config_IMPORTS ${config_IMPORTS})

    # 强制链接目标
    foreach (depend ${config_FORCE_DEPENDS})
        get_target_property(target_type ${depend} TYPE)

        if (target_type STREQUAL STATIC_LIBRARY)
            list(APPEND FORCE_DEPENDS_TARGET_STATIC ${depend})
        else ()
            list(APPEND FORCE_DEPENDS_TARGET_SHARED ${depend})
        endif ()
    endforeach ()

    set(FORCE_DEPENDS_TARGET
            "-Wl,--whole-archive ${FORCE_DEPENDS_TARGET_STATIC} -Wl,--no-whole-archive"
            "-Wl,--no-as-needed ${FORCE_DEPENDS_TARGET_SHARED} -Wl,--as-needed")

    # 外部依赖
    poker_find_packages(${config_IMPORTS_ALL} COMPONENTS ${config_IMPORTS_COMPONENTS} AS import_results)

    # 整合各个链接项，完成依赖传递
    set(poker_target_depends_interface ${config_DEPENDS_INTERFACE} ${config_IMPORTS_INTERFACE})
    set(poker_target_depends_private ${config_DEPENDS_PRIVATE} ${config_IMPORTS_PRIVATE} ${config_LIBRARY})
    set(poker_target_depends_public ${FORCE_DEPENDS_TARGET} ${config_DEPENDS_Export} ${config_IMPORTS_Export})

    if (${is_interface})
        target_link_libraries(${target_name} INTERFACE ${poker_target_depends_public} ${poker_target_depends_interface})
    else ()
        target_link_libraries(${target_name} PRIVATE ${poker_target_depends_private})
        target_link_libraries(${target_name} INTERFACE ${poker_target_depends_interface})
        target_link_libraries(${target_name} PUBLIC ${poker_target_depends_public})
    endif ()

    # step 6: 安装该目标

    # 安装头文件
    install(DIRECTORY include/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.tpp"
    )

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

    configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/Config.cmake.in
            " ${CMAKE_CURRENT_BINARY_DIR}/${target_name}Config.cmake"
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

    # step 7: 打包目标
endfunction()