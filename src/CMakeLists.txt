add_library(matrix_lib INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/matrix.hpp")

target_include_directories(matrix_lib INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")


add_executable(${CMAKE_PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE matrix_lib)
