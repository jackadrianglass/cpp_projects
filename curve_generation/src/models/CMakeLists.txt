add_library(
        models STATIC
        src/camera.cc
        src/grid_outline.cc
        src/curve.cc
        src/orientation.cc
        src/revolved_surface.cc
        src/2d_curve_algorithms.cc
        src/3d_surface_algorithms.cc
        src/grid.cc
)

set_project_warnings(models)
target_include_directories(models PUBLIC include)

find_package(spdlog CONFIG REQUIRED)

target_link_libraries(
        models
    PUBLIC
        gl_wrapper
    PRIVATE
        spdlog::spdlog
        project_options
)
