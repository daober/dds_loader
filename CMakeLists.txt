cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(dds_loader)

#GTEST SPECIFIC START
#set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CMakeLists.txt.in
#        googletest-download/CMakeLists.txt)
#execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
#        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
#execute_process(COMMAND ${CMAKE_COMMAND} --build .
#        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

#add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
#        ${CMAKE_BINARY_DIR}/googletest-build)

#enable_testing()
#GTEST SPECIFIC END

#add_subdirectory(test)
add_subdirectory(exports)

##
### Source definitions ###
##
MESSAGE("...")
MESSAGE("BUILDING " ${PROJECT_NAME})
SET(HEADER ${PROJECT_SOURCE_DIR}/include/dds_loader.h)
SET(SRC ${PROJECT_SOURCE_DIR}/src/dds_loader.c)

add_library(dds SHARED ${SRC})
add_library(dds::loader ALIAS dds)

target_include_directories(dds PRIVATE
                        src
                        PUBLIC
	                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
                        $<INSTALL_INTERFACE:include> 
)

install(DIRECTORY include DESTINATION .)

install(TARGETS dds     
	EXPORT Targets 
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        )

