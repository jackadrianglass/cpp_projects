add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC Catch2)
target_link_libraries(catch_main PRIVATE project_options)

add_subdirectory(2d_fractals)