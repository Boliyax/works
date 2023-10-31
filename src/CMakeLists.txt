


find_package(BOOST COMPONENTS system asio regex REQUIRED)


add_executable(${PROJECT_NAME} server.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES} ws2_32 wsock32)
include_directories(${Boost_INCLUDE_DIRS} )

add_library(command_set INTERFACE command_set.hpp)
target_include_directories(command_set INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(output INTERFACE output.hpp)
target_include_directories(output INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(input_controller INTERFACE input_controller.hpp)
target_include_directories(input_controller INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(libasync STATIC async.cpp async.h)
target_link_libraries(libasync PRIVATE command_set output input_controller)
target_include_directories(libasync INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(${PROJECT_NAME} PRIVATE libasync)