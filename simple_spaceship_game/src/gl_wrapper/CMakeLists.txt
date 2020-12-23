add_library(
        gl_wrapper STATIC
        src/handle.cc
        src/shader.cc
        src/shader_program.cc
        src/vertex_array.cc
        src/vertex_buffer.cc
        src/gpu_geometry.cc
        src/texture.cc
        src/debug.cc
)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Microsoft.GSL CONFIG REQUIRED)

find_path(STB_INCLUDE_DIRS "stb.h")
target_include_directories(gl_wrapper PRIVATE ${STB_INCLUDE_DIRS})

target_include_directories(gl_wrapper PUBLIC include)
target_link_libraries(
        gl_wrapper
        PUBLIC
        glad::glad
        glm
        spdlog::spdlog
        OpenGL::GL
        Microsoft.GSL::GSL
)
