add_library(
        gl_shapes STATIC
        src/square.cc
)

target_include_directories(gl_shapes PUBLIC include)
# set_project_warnings(gl_shapes)

find_package(glm REQUIRED)

target_link_libraries(
        gl_shapes
        PUBLIC
        gl_wrapper
        glm
)