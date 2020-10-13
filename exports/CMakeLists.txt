include(CMakePackageConfigHelpers)

configure_package_config_file(LibraryConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION .
    )
    
install(
    EXPORT dds_loader-targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE dds::
    DESTINATION cmake
    )
    
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    DESTINATION cmake
    )
