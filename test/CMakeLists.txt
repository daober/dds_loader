MESSAGE("BUILDING TESTS")
MESSAGE("...")

find_package(GTest REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/include")

add_executable(dds_tests dds.cpp main.cpp)
target_link_libraries(dds_tests GTest::Main dds_loader)
add_test(NAME dds_test COMMAND functionality_test)


#foreach(file ${tests})
#  set(name dds_tests)
#  get_filename_component(name ${file} NAME_WE)
#  add_executable("${name}_tests"
#    ${sources}
#    ${file}
#    "${PROJECT_SOURCE_DIR}/test/main.cpp")
#  target_link_libraries("${name}_tests" gtest_main)
#  add_test(NAME ${name} COMMAND "${name}_tests")
#endforeach()
