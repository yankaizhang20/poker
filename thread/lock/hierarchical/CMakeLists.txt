add_library(thread-lock-hierarchical src/hierarchical_mutex.cpp)
target_include_directories(thread-lock-hierarchical PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# TODO: 换成使用 target_sources() 安装头文件目录
install(FILES include/hierarchical_mutex.h DESTINATION include)
install(TARGETS thread-lock-hierarchical
        EXPORT ThreadLockHierarchicalTarget
        DESTINATION lib
)
install(EXPORT ThreadLockHierarchicalTarget
        FILE ThreadLockHierarchicalTarget.cmake
        DESTINATION lib/cmake/TheadLock
)


include(CMakePackageConfigHelpers)

set(target_profile ThreadLockHierarchicalTarget)

configure_package_config_file(${CMAKE_SOURCE_DIR}/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/ThreadLockHierarchicalTarget.cmake"
        INSTALL_DESTINATION "lib/cmake/TheadLock"
        NO_SET_AND_CHECK_MACRO
        NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/ThreadLockHierarchicalConfigVersion.cmake"
        VERSION "${poker_VERSION_MAJOR}.${poker_VERSION_MINOR}"
        COMPATIBILITY AnyNewerVersion
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/ThreadLockHierarchicalConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/ThreadLockHierarchicalConfigVersion.cmake
        DESTINATION lib/cmake/TheadLock
)
