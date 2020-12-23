add_library(
        models STATIC
        src/camera.cc
        src/camera_orientation.cc
        src/camera_controller.cc
        src/grid_outline.cc
        src/curve.cc
        src/revolved_surface.cc
        src/2d_curve_algorithms.cc
        src/3d_surface_algorithms.cc
        src/grid.cc)

find_package(spdlog CONFIG REQUIRED)

set_project_warnings(models)
target_include_directories(models PUBLIC include)

target_link_libraries(
        models
    PUBLIC
        gl_wrapper
    PRIVATE
        spdlog::spdlog
        project_options
)
