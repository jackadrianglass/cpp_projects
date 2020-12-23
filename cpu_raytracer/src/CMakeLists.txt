add_subdirectory(gl_wrapper)
add_subdirectory(models)
add_subdirectory(glfw_wrapper)

add_executable(cpu_raytracer
        main.cc
        callbacks.cc callbacks.h
        material.cc material.h
        scene.cc scene.h
        ray_trace.cc ray_trace.h
        image_buffer.cc image_buffer.h
        lighting.h)

set_project_warnings(cpu_raytracer)

find_package(spdlog CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")
target_include_directories(cpu_raytracer PRIVATE ${STB_INCLUDE_DIRS})

target_link_libraries(
        cpu_raytracer

        PRIVATE
        project_options
        gl_wrapper
        glfw_wrapper
        models
        spdlog::spdlog
    )
