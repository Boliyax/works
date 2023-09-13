find_package(Boost COMPONENTS system filesystem program_options regex  REQUIRED)

set(${CXX_STANDARD} 20)

include_directories(${Boost_INCLUDE_DIRS})


add_library(hash OBJECT hash.cpp hash.hpp)

add_library(multi_comparator OBJECT multi_comparator.cpp multi_comparator.hpp)

add_library(mask_filter OBJECT mask_filter.cpp mask_filter.hpp)

add_library(file_searcher OBJECT file_searcher.cpp file_searcher.hpp )

add_library(main_mechanism OBJECT main_mechanism.cpp main_mechanism.hpp)

add_executable(${PROJECT_NAME} main.cpp main_mechanism file_searcher mask_filter multi_comparator hash)
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY} )