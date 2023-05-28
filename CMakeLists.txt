cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(alloc_example VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Release)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Werror -Wall -Wextra -Wpedantic)

add_subdirectory(src)
add_subdirectory(test)


install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)


enable_testing()
add_test(test unit_tests)




