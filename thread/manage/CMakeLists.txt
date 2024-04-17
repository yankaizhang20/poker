add_library(thread-manage src/manage.cpp)
target_include_directories(thread-manage INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(thread-manage PUBLIC thread-lock-hierarchical)

add_executable(thread-manage_test test/main.cpp)
target_link_libraries(thread-manage_test
        PUBLIC poker_compiler_flag
        thread-manage
)

install(FILES include/manage.h
        DESTINATION include
)
install(TARGETS thread-manage
        EXPORT ThreadManageTarget
        DESTINATION lib
)
install(TARGETS thread-manage_test DESTINATION bin)

install(EXPORT ThreadManageTarget
        FILE ThreadManageTarget.cmake
        DESTINATION lib/cmake/ThreadManage
)

include(CMakePackageConfigHelpers)


set(target_profile ThreadManageTarget)
configure_package_config_file(${CMAKE_SOURCE_DIR}/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/ThreadManageConfig.cmake"
        INSTALL_DESTINATION "lib/cmake/ThreadManage"
        NO_SET_AND_CHECK_MACRO
        NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/ThreadManageConfigVersion.cmake"
        VERSION "${poker_VERSION_MAJOR}.${poker_VERSION_MINOR}"
        COMPATIBILITY AnyNewerVersion
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/ThreadManageConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/ThreadManageConfigVersion.cmake
        DESTINATION lib/cmake/ThreadManage
)