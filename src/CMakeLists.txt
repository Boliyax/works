cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

add_library(print_ip_lib INTERFACE print_ip.hpp)
target_include_directories(print_ip_lib INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE print_ip_lib)