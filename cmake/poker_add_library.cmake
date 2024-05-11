###################################################################################
# poker_add_library(<target>
#                   [SHARED]
#                   [DEPENDS        <components [Inner components...]>]
#                   [IMPORTS        <components [Inner components...]>]
#                   [INCLUDE        <components [Inner components...]>]
#                   [FORCE_DEPENDS  components...]
#                   [LIBRARY        components...]
#                   [  SRC          components...]
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
# DEPENDS: 依赖本工程内的其他 target，默认将传递该依赖。通过指定 Inner 阻止依赖传递
#
# FORCE_DEPENDS: 强制链接本工程内的其他 target。不继承其使用说明
#
# IMPORTS: 依赖系统中的外部库，默认将传递该依赖。通过指定 Inner 组织依赖传递
#
# LIBRARY: 直接依赖系统中的外部库
#
# INCLUDE: 指定头文件搜索目录，默认传递该目录。通过指定 Inner 组织依赖传递 TODO: 导出目录在生成表达式中应为相对目录？
#
# SRC: 额外添加源文件目录
###################################################################################

function(poker_add_library target_name)
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

    # 解析 INCLUDE 后的 Inner 参数
    poker_split_arguments(config_INCLUDE ${config_INCLUDE})

    if (${is_interface})
        # 接口模式中使用 INCLUDE Inner 为错误用法
        if (NOT "${config_INCLUDE_Inner}" STREQUAL "")
            message(FATAL_ERROR "shouldn't use INCLUDE with Inner in INTERFACE target!")
        endif ()

        target_include_directories(${target_name} INTERFACE
                $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
                $<INSTALL_INTERFACE:include,${config_INCLUDE_Export}>
        )
    else ()
        target_include_directories(${target_name} PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include,${config_INCLUDE_Inner}>
                $<INSTALL_INTERFACE:include,${config_INCLUDE_Export}>
        )
    endif ()

    # step 5: 添加依赖说明

    # 解析 DEPENDS、IMPORTS 后的 Inner 参数
    poker_split_arguments(config_DEPENDS ${config_DEPENDS})
    poker_split_arguments(config_IMPORTS ${config_IMPORTS})

    # 为强制链接目标增加链接选项
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

    set(poker_target_depends_private ${config_DEPENDS_Inner} ${config_IMPORTS_Inner})
    set(poker_target_depends_public ${FORCE_DEPENDS_TARGET} ${config_DEPENDS_Export} ${config_IMPORTS_Export})

    if (${is_interface})
        target_link_libraries(${target_name} INTERFACE ${poker_target_depends_public})
    else ()
        target_link_libraries(${target_name} PRIVATE ${poker_target_depends_private})
        target_link_libraries(${target_name} PUBLIC ${poker_target_depends_public})
    endif ()

    # step 6: 安装该目标
    install(FILES include/manage.h
            DESTINATION include
    )
    install(TARGETS thread-manage
            EXPORT thread-manageTarget
            DESTINATION lib
    )
    install(TARGETS thread-manage_test DESTINATION bin)

    install(EXPORT thread-manageTarget
            FILE thread-manageTarget.cmake
            DESTINATION lib/cmake/target
    )

    # step 7: 打包目标
    include(CMakePackageConfigHelpers)

    set(target_profile thread-manageTarget)
    configure_package_config_file(${CMAKE_SOURCE_DIR}/Config.cmake.in
            " ${CMAKE_CURRENT_BINARY_DIR}/thread-manageConfig.cmake"
            INSTALL_DESTINATION "lib/cmake/target"
            NO_SET_AND_CHECK_MACRO
            NO_CHECK_REQUIRED_COMPONENTS_MACRO
    )

    write_basic_package_version_file(
            "${CMAKE_CURRENT_BINARY_DIR}/thread-manageConfigVersion.cmake"
            VERSION "${poker_VERSION_MAJOR}.${poker_VERSION_MINOR}"
            COMPATIBILITY AnyNewerVersion
    )

    install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/thread-manageConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/thread-manageConfigVersion.cmake
            DESTINATION lib/cmake/target
    )
endfunction()