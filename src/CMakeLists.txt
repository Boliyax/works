cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

add_library(command_set INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/command_set.hpp")
target_include_directories(command_set INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(output INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/output.hpp")
target_include_directories(output INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(input_controller INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/input_controller.hpp")
target_include_directories(input_controller INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(${PROJECT_NAME} PRIVATE command_set output input_controller)

