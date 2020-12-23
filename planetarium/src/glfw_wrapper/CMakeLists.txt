add_library(
        glfw_wrapper STATIC
        src/callback_interface.cc
        src/window.cc
        src/scene.cc
)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

set_project_warnings(glfw_wrapper)
target_include_directories(glfw_wrapper PUBLIC include)

target_link_libraries(
        glfw_wrapper
    PUBLIC
        glfw
        imgui::imgui
        glm
    PRIVATE
        gl_wrapper
        spdlog::spdlog
        project_options
)
