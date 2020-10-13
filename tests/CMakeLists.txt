MESSAGE("BUILDING TESTS")
MESSAGE("...")

find_package(GTest REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/include")

add_executable(dds_tests dds.cpp main.cpp)
target_link_libraries(dds_tests GTest::Main dds_loader)
add_test(NAME dds_test COMMAND functionality_test)
