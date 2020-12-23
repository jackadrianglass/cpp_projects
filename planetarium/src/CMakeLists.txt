add_subdirectory(gl_wrapper)
add_subdirectory(models)
add_subdirectory(glfw_wrapper)

add_executable(planetarium
        main.cc
        callbacks.cc callbacks.h
        sphere.cc sphere.h
        planetoid.cc planetoid.h
        transform.cc transform.h)

set_project_warnings(planetarium)

find_package(OpenGL REQUIRED)
find_package(spdlog CONFIG REQUIRED)

target_link_libraries(
        planetarium

        PRIVATE
        project_options
        gl_wrapper
        glfw_wrapper
        models
        spdlog::spdlog
    )
