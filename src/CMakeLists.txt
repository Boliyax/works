cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

add_library(ip_lib OBJECT ip.cpp)
target_include_directories(ip_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(split_lib OBJECT split.cpp)
target_include_directories(split_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ip_lib split_lib)