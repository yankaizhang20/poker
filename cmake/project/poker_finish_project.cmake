macro(poker_finish_project)
    # step 1: 安装工程配置文件
    include(CMakePackageConfigHelpers)

    # 工程配置文件名
    set(project_config_file_name "${PROJECT_NAME}Config.cmake")
    # 工程配置文件路径
    set(project_config_file_path "${CMAKE_INSTALL_LIBDIR}/cmake/")
    # 工程版本文件名
    set(project_version_file_name "${PROJECT_NAME}ConfigVersion.cmake")

    configure_package_config_file(${poker_template_dir}/project_config.cmake
            "${CMAKE_CURRENT_BINARY_DIR}/${project_config_file_name}"
            INSTALL_DESTINATION "${project_config_file_path}"
            NO_SET_AND_CHECK_MACRO
            NO_CHECK_REQUIRED_COMPONENTS_MACRO
    )

    write_basic_package_version_file(
            "${CMAKE_CURRENT_BINARY_DIR}/${project_version_file_name}"
            VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}"
            COMPATIBILITY AnyNewerVersion
    )

    install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/${project_config_file_name}
            ${CMAKE_CURRENT_BINARY_DIR}/${project_version_file_name}
            DESTINATION "${project_config_file_path}"
    )

    # step 2: 构建安装包
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "POKER")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "POKER IS A BASE C++ LIBRARY")
    set(CPACK_DEBIAN_PACKAGE_VERSION_MAJOR "${poker_VERSION_MAJOR}")
    set(CPACK_DEBIAN_PACKAGE_VERSION_MINOR "${poker_VERSION_MINOR}")
    set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6), libc6 (<< 3)")
    include(CPack)
endmacro()