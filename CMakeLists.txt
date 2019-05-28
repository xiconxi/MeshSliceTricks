cmake_minimum_required(VERSION 3.7)
project(SliceVisual)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(Externals/gl3w)
add_subdirectory(Externals/glfw-3.3)

include_directories(Externals/glm-0.9.9.5/)
include_directories(Externals/imgui-1.70)
include_directories(Externals/gl3w/include)

add_executable(SliceVisual main.cpp GLSLShader.cpp Mesh.cpp
        Externals/imgui-1.70/imgui.cpp
        Externals/imgui-1.70/imgui_demo.cpp
        Externals/imgui-1.70/imgui_draw.cpp
        Externals/imgui-1.70/imgui_widgets.cpp
        Externals/imgui-1.70/examples/imgui_impl_glfw.cpp
        Externals/imgui-1.70/examples/imgui_impl_opengl3.cpp
        )
target_link_libraries(SliceVisual glfw gl3w)