###################################################################################
# poker_add_library(target [DEPENDS ...] [IMPORTS ...] [LIBRARY ...] [INCLUDE ...] [SRC ...])
###################################################################################
# 1.添加 src 目录下的同名目录下的全部源文件
# 2.若没有源文件，则作为 interface target
# 3.将 include 目录作为本 target 的使用说明（specification）
###################################################################################
# DEPENDS: 依赖本工程内的其他 target
# FORCE_DEPENDS: 强制链接本工程内的其他 target
# IMPORTS: 依赖提供了 config 的外部库
# LIBRARY: 直接依赖外部库
# INCLUDE: 指定头文件搜索目录
# SRC: 额外添加源文件目录
###################################################################################

function(poker_add_library target_name)
    # step 1: 解析位置参数
    poker_parse_arguments(config "" "" "" ${ARGN})

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
        add_library(${target_name} ${poker_target_sources})
    endif ()

    # step 4: 设置头文件搜索路径 TODO: 缺少 include 的 PRIVATE 属性判断
    if (${is_interface})
        target_include_directories(${target_name} INTERFACE
                $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include,${config_INCLUDE}>
                $<INSTALL_INTERFACE:include,${config_INCLUDE}>
        )
    else ()
        target_include_directories(${target_name} PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include,${config_INCLUDE}>
                $<INSTALL_INTERFACE:include,${config_INCLUDE}>
        )
    endif ()

    # step 5: 添加依赖说明
    set(poker_target_depends)

    target_link_libraries(${target_name} PUBLIC ${poker_target_depends})

    # 安装该目标
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

    # 打包目标
    include(CMakePackageConfigHelpers)

    set(target_profile thread-manageTarget)
    configure_package_config_file(${CMAKE_SOURCE_DIR}/Config.cmake.in
            "${CMAKE_CURRENT_BINARY_DIR}/thread-manageConfig.cmake"
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