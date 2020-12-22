#add_subdirectory(math)
add_subdirectory(gl_wrapper)
add_subdirectory(gl_shapes)

add_executable(assignment2 main.cc window.cc window.h)
#set_project_warnings(assignment2)

find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

find_package(OpenGL REQUIRED)
target_link_libraries(
        assignment2

        PRIVATE
        project_options
        glfw
        imgui::imgui
        gl_wrapper
    )
