cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(VERSION_PATCH "1" CACHE INTERNAL "version_patch")
set(VERSION "0.0.${VERSION_PATCH}")

project(helloworld VERSION ${VERSION})

configure_file(config.h.in config.h)

add_library(${PROJECT_NAME} STATIC lib.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_BINARY_DIR}")

set(EXE_NAME "${PROJECT_NAME}_cli")
add_executable(${EXE_NAME} main.cpp)
target_link_libraries(${EXE_NAME} PRIVATE ${PROJECT_NAME})

add_executable(my_test test.cpp)
find_package(GTest REQUIRED)
target_link_libraries(my_test PRIVATE ${PROJECT_NAME} GTest::gtest)

set_target_properties(${PROJECT_NAME} ${EXE_NAME} my_test PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
install(TARGETS ${EXE_NAME} RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

enable_testing()
add_test(test ./my_test)