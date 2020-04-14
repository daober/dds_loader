cmake_minimum_required(VERSION 3.1)

project(dds_loader)

set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CMakeLists.txt.in
        googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build)

enable_testing()

add_subdirectory(test)
add_subdirectory(exports)

##
### Source definitions ###
##
SET(HEADERS ${PROJECT_SOURCE_DIR}/include/dds_loader.h)

#include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} STATIC ${sources} ${HEADERS})

set(lib_dest "lib/")
set(include_dest "include/")

target_include_directories(${PROJECT_NAME} PRIVATE
                        src
                        PUBLIC
	                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # for config_impl.hpp when building
                        $<INSTALL_INTERFACE:include> # for client in install mode
)

#set(export_config_file ${CMAKE_CURRENT_SOURCE_DIR}/exports)
install(DIRECTORY include DESTINATION .)


install(TARGETS ${PROJECT_NAME} 
        EXPORT Targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        )

