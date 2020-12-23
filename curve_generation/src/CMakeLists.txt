add_subdirectory(gl_wrapper)
add_subdirectory(models)
add_subdirectory(glfw_wrapper)

add_executable(curve_generation
        main.cc
        callbacks.cc callbacks.h
        curve_scene.cc curve_scene.h
        surface_scene.cc surface_scene.h)

#set_project_warnings(curve_generation)

find_package(spdlog CONFIG REQUIRED)

find_package(OpenGL REQUIRED)
target_link_libraries(
        curve_generation

        PRIVATE
        project_options
        gl_wrapper
        glfw_wrapper
        models
        spdlog::spdlog
    )
