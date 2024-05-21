poker_add_library(thread-manage DEPENDS thread-lock-hierarchical)

#add_library(thread-manage src/manage.cpp)
#target_include_directories(thread-manage INTERFACE
#        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
#        $<INSTALL_INTERFACE:include>
#)
#target_link_libraries(thread-manage PUBLIC thread-lock-hierarchical)

add_executable(thread-manage_test test/main.cpp)
target_link_libraries(thread-manage_test
        PUBLIC poker_compiler_flag
        thread-manage
)

#install(FILES include/manage.h
#        DESTINATION include
#)
#install(TARGETS thread-manage
#        EXPORT thread-manageTarget
#        DESTINATION lib
#)
install(TARGETS thread-manage_test DESTINATION bin)

#install(EXPORT thread-manageTarget
#        FILE thread-manageTarget.cmake
#        DESTINATION lib/cmake/target
#)

#include(CMakePackageConfigHelpers)
#
#
#set(target_profile thread-manageTarget)
#configure_package_config_file(${CMAKE_SOURCE_DIR}/Config.cmake.in
#        "${CMAKE_CURRENT_BINARY_DIR}/thread-manageConfig.cmake"
#        INSTALL_DESTINATION "lib/cmake/target"
#        NO_SET_AND_CHECK_MACRO
#        NO_CHECK_REQUIRED_COMPONENTS_MACRO
#)
#
#write_basic_package_version_file(
#        "${CMAKE_CURRENT_BINARY_DIR}/thread-manageConfigVersion.cmake"
#        VERSION "${poker_VERSION_MAJOR}.${poker_VERSION_MINOR}"
#        COMPATIBILITY AnyNewerVersion
#)
#
#install(FILES
#        ${CMAKE_CURRENT_BINARY_DIR}/thread-manageConfig.cmake
#        ${CMAKE_CURRENT_BINARY_DIR}/thread-manageConfigVersion.cmake
#        DESTINATION lib/cmake/target
#)