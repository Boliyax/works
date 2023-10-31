cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

find_package(BOOST REQUIRED COMPONENTS system asio regex)



add_executable(server server.cpp)

target_link_libraries(server PRIVATE ${Boost_LIBRARIES} ws2_32 wsock32)
include_directories(${Boost_INCLUDE_DIRS} )

add_library(database STATIC database.cpp database.hpp)
add_library(command_interpreter STATIC command_interpreter.cpp command_interpreter.hpp)
target_link_libraries(command_interpreter INTERFACE database)
target_link_libraries(server PRIVATE command_interpreter)