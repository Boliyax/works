cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

add_library(smart_bitset INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/smart_bitset.hpp")
add_library(my_allocator INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/my_allocator.hpp")
add_library(compact_vector INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/compact_vector.hpp")

target_include_directories(smart_bitset INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(my_allocator INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(compact_vector INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(my_allocator INTERFACE smart_bitset)

add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE my_allocator compact_vector)
