cmake_minimum_required(VERSION 3.7)
project(SliceVisual)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(External/gl3w)
add_subdirectory(External/glfw3.3)

include_directories(External/glm)
include_directories(External/imgui)
include_directories(External/gl3w/include)

add_executable(SliceVisual main.cpp GLSLShader.cpp Mesh.cpp
        External/imgui/imgui.cpp
        External/imgui/imgui_demo.cpp
        External/imgui/imgui_draw.cpp
        External/imgui/imgui_widgets.cpp
        External/imgui/examples/imgui_impl_glfw.cpp
        External/imgui/examples/imgui_impl_opengl3.cpp
        )
target_link_libraries(SliceVisual glfw gl3w)