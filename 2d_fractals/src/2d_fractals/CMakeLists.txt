
add_library(
        2d_fractals STATIC
        src/fractal_math.cc
        src/cpu_geometry.cc
        src/serpinsky_triangle.cc
        src/squares_and_diamonds.cc
        src/koch_snowflake.cc
        src/dragon_curve.cc 
        src/hilbert_curve.cc
        )

target_include_directories(2d_fractals PUBLIC include)
# set_project_warnings(2d_fractals)

find_package(glm REQUIRED)
find_package(spdlog REQUIRED)
target_link_libraries(
        2d_fractals
        PUBLIC
        project_options
        glm
        spdlog::spdlog
        gl_shapes
)