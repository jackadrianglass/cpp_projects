add_executable(
        2d_fractals_tests
        fractal_math.test.cc
        serpinsky_triangle.test.cc
        squares_and_diamonds.test.cc
)

target_link_libraries(
        2d_fractals_tests
        PRIVATE
        2d_fractals
        catch_main
)