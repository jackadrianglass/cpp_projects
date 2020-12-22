add_subdirectory(gl_wrapper)
add_subdirectory(gl_shapes)
add_subdirectory(2d_fractals)
add_subdirectory(2d_fractal_scenes)

add_executable(assignment1 main.cpp application.cc application.h)

file(COPY shaders DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(imgui CONFIG REQUIRED)

target_link_libraries(
        assignment1

        PRIVATE
        project_options
        glfw
        imgui::imgui
        gl_wrapper
        2d_fractal_scenes
    )
