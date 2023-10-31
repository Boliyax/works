find_package(Boost 1.82.0 COMPONENTS system regex REQUIRED)

add_executable(${PROJECT_NAME} server.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})
include_directories(${Boost_INCLUDE_DIRS} )

add_library(database STATIC database.cpp database.hpp)
add_library(command_interpreter STATIC command_interpreter.cpp command_interpreter.hpp)
target_link_libraries(command_interpreter INTERFACE database)
target_link_libraries(${PROJECT_NAME} PRIVATE command_interpreter)