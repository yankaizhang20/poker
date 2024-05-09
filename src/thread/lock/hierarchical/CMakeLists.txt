add_library(thread-lock-hierarchical src/hierarchical_mutex.cpp)
target_include_directories(thread-lock-hierarchical PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# TODO: 换成使用 target_sources() 安装头文件目录
install(FILES include/hierarchical_mutex.h DESTINATION include)
install(TARGETS thread-lock-hierarchical
        EXPORT thread-lock-hierarchicalTarget
        DESTINATION lib
)
install(EXPORT thread-lock-hierarchicalTarget
        FILE thread-lock-hierarchicalTarget.cmake
        DESTINATION lib/cmake/target
)


include(CMakePackageConfigHelpers)

set(target_profile thread-lock-hierarchicalTarget)

configure_package_config_file(${CMAKE_SOURCE_DIR}/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/thread-lock-hierarchicalConfig.cmake"
        INSTALL_DESTINATION "lib/cmake/target"
        NO_SET_AND_CHECK_MACRO
        NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/thread-lock-hierarchicalConfigVersion.cmake"
        VERSION "${poker_VERSION_MAJOR}.${poker_VERSION_MINOR}"
        COMPATIBILITY AnyNewerVersion
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/thread-lock-hierarchicalConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/thread-lock-hierarchicalConfigVersion.cmake
        DESTINATION lib/cmake/target
)
