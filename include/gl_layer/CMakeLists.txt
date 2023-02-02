add_library(gl_layer_INC 
    INTERFACE 
        GLLayer.hpp 
        Shader.hpp 
        Framebuffer.hpp 
        Quad.hpp 
        Texture2D.hpp 
        Texture2DArray.hpp
        Workspace.hpp
        ShadersRepository.hpp
        Sampler.hpp
        StorageBufferObject.hpp
)
target_include_directories(gl_layer_INC INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(gl_layer_INC INTERFACE common_INC)