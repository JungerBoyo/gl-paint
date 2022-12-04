find_package(imgui REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(stb REQUIRED)

add_library(imgui_bindings
    imgui_bindings/imgui_impl_glfw.cpp
    imgui_bindings/imgui_impl_opengl3.cpp
    imgui_bindings/imgui_impl_glfw.h
    imgui_bindings/imgui_impl_opengl3.h
)
target_link_libraries(imgui_bindings PRIVATE imgui::imgui glfw::glfw glad::glad)
target_include_directories(imgui_bindings INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(imgui_bindings PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

add_library(stbi stbi/stbi.cpp)
target_link_libraries(stbi PUBLIC stb::stb)