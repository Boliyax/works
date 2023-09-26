cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

add_executable(test main.cpp)
target_link_libraries(test PRIVATE libasync)
