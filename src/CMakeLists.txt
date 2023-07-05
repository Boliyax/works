add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
add_library(builder OBJECT "${CMAKE_CURRENT_SOURCE_DIR}/graphic_primitives_builder.cpp")
add_library(primitives OBJECT "${CMAKE_CURRENT_SOURCE_DIR}/graphic_primitives.cpp")
add_library(interpreter OBJECT "${CMAKE_CURRENT_SOURCE_DIR}/interpreter.cpp")
add_library(canvas OBJECT "${CMAKE_CURRENT_SOURCE_DIR}/canvas.cpp")

target_link_libraries(vector_editor PRIVATE builder primitives interpreter canvas)