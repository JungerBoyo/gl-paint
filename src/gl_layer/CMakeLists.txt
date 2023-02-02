find_package(glad REQUIRED)
find_package(spdlog REQUIRED)
find_package(glfw3 REQUIRED)

add_library(gl_layer_IMPL 
  STATIC 
    GLLayer.cpp 
    Shader.cpp 
    Framebuffer.cpp 
    Quad.cpp 
    Texture2D.cpp 
    Texture2DArray.cpp
    Workspace.cpp
    ShadersRepository.cpp
    Sampler.cpp
    StorageBufferObject.cpp
)
target_link_libraries(gl_layer_IMPL PUBLIC gl_layer_INC)

target_link_system_libraries(gl_layer_IMPL
  PRIVATE
    utils_IMPL
    glad::glad
    spdlog::spdlog
    glfw::glfw
    SHCONFIG
)