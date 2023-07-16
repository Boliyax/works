cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

find_package(GTest REQUIRED)

add_executable(unit_tests ${CMAKE_CURRENT_SOURCE_DIR}/main_test.cpp)
target_link_libraries(unit_tests PRIVATE ip_lib split_lib)
target_link_libraries(unit_tests PRIVATE GTest::gtest GTest::gtest_main)
