cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(async VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -pedantic -Werror)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Release")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")