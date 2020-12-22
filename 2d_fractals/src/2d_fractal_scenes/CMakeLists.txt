add_library(
        2d_fractal_scenes STATIC
        src/scene.cc
        src/serpinsky_scene.cc
        src/square_diamond_scene.cc
        src/koch_snowflake_scene.cc
        src/dragon_curve_scene.cc include/dragon_curve_scene.h src/hilbert_scene.cc include/hilbert_scene.h)
target_include_directories(2d_fractal_scenes PUBLIC include)

# set_project_warnings(2d_fractal_scenes)
target_link_libraries(
        2d_fractal_scenes
        PUBLIC
        2d_fractals
        gl_wrapper
)