include(CMakePackageConfigHelpers)

configure_package_config_file(LibraryConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION .
    )
    
install(
    EXPORT Targets
    FILE ${PROJECT_NAME}Targets.cmake
    DESTINATION cmake
    )
    
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    DESTINATION cmake
    )