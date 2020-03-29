cmake_minimum_required(VERSION 3.1)

project(SampleGTestTemplate)

set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)

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


##
### Source definitions ###
##
SET(HEADERS ${PROJECT_SOURCE_DIR}/include/dds_loader.h)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${sources} ${HEADERS} )
