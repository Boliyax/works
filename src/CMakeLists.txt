cmake_minimum_required(VERSION 3.10 FATAL_ERROR)


find_package(BOOST REQUIRED COMPONENTS system asio regex)


add_executable(server server.cpp)
target_link_libraries(server PRIVATE ${Boost_LIBRARIES} ws2_32 wsock32)
include_directories(${Boost_INCLUDE_DIRS} )

add_library(command_set INTERFACE command_set.hpp)
target_include_directories(command_set INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(output INTERFACE output.hpp)
target_include_directories(output INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(input_controller INTERFACE input_controller.hpp)
target_include_directories(input_controller INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(libasync SHARED async.cpp async.h)
target_link_libraries(libasync PRIVATE command_set output input_controller)
target_include_directories(libasync INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(server PRIVATE libasync)