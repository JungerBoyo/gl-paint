#include <array>

#include <Shader.hpp>
#include <Window.hpp>
#include <config.hpp>

#include <glad/glad.h>

#include <stb_image.h>
#include <spdlog/spdlog.h>

#include <imgui.h>
#include <imgui_bindings/imgui_impl_glfw.h>
#include <imgui_bindings/imgui_impl_opengl3.h>

using namespace template_project;

int main() {
    // intialize glfw3 + glad
    Window window(cmake::project_name, 640, 480, 
    [](int err_code, const char* message){
        spdlog::error("[GLFW3]{}:{}", err_code, message);
    },[](uint32_t, uint32_t, uint32_t id, uint32_t, int, const char *message, const void *){
        spdlog::error("[OPENGL]{}:{}", id, message);
    });
    // initialize imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(window.native()), true);
    ImGui_ImplOpenGL3_Init("#version 450");

    // App data
    constexpr std::array<float, 6UL*(2UL+2UL)> quad_vertices{{
        -.5F,-.5F, 0.F, 0.F,
         .5F,-.5F, 1.F, 0.F,
         .5F, .5F, 1.F, 1.F,
        -.5F,-.5F, 0.F, 0.F,
         .5F, .5F, 1.F, 1.F,
        -.5F, .5F, 0.F, 1.F,
    }};

    struct {
        float quad_scale{ 1.F };
        float aspect_ratio{ 640.F/480.F };
    } ubo_data;
    std::array<float, 3> clear_color{{0.F, 0.F, 0.F}};
    
    std::uint32_t quad_vbo_id{ 0U };
    std::uint32_t quad_vao_id{ 0U };
    std::uint32_t quad_ubo_id{ 0U };
    std::uint32_t quad_tex_id{ 0U };
    
    Shader basic_shader(
        Shader::Type::VERTEX_FRAGMENT,
        {
            "shaders/bin/basic_shader/vert.spv",
            "shaders/bin/basic_shader/frag.spv",
        }
    );
    // create VBO and initialize
    glCreateBuffers(1, &quad_vbo_id);
    glNamedBufferStorage(
        quad_vbo_id, 
        sizeof(quad_vertices), 
        static_cast<const void*>(quad_vertices.data()), 
        0
    );
    // create VAO and configure layout
    glCreateVertexArrays(1, &quad_vao_id);
    {
        constexpr std::uint32_t attribs_binding{ 0 };
        constexpr std::array<std::uint32_t, 2> attrib_locations{{
            SHCONFIG_IN_POSITION_LOCATION,
            SHCONFIG_IN_TEXCOORD_LOCATION
        }};
        constexpr std::uint32_t base_size{ sizeof(float) };
        constexpr std::uint32_t num_components{ 2 };
        std::uint32_t relative_offset{ 0 };
        for (const auto attrib_location : attrib_locations) {
            glEnableVertexArrayAttrib(quad_vao_id, attrib_location);
            glVertexArrayAttribFormat(
                quad_vao_id, 
                attrib_location,
                num_components,
                GL_FLOAT,
                GL_FALSE,
                relative_offset
            );
            glVertexArrayAttribBinding(quad_vao_id, attrib_location, attribs_binding);
            relative_offset += num_components * base_size;
        }
        glVertexArrayBindingDivisor(quad_vao_id, attribs_binding, 0);
        glVertexArrayVertexBuffer(
            quad_vao_id, 
            attribs_binding, 
            quad_vbo_id, 0, 
            static_cast<int>(relative_offset)
        );
    }
    // create UBO 
    glCreateBuffers(1, &quad_ubo_id);
    glNamedBufferStorage(
        quad_ubo_id, 
        sizeof(ubo_data), 
        static_cast<const void*>(&ubo_data), 
        GL_DYNAMIC_STORAGE_BIT
    );
    glBindBufferBase(GL_UNIFORM_BUFFER, SHCONFIG_UBO_BINDING, quad_ubo_id);
    // create Texture and load image
    glCreateTextures(GL_TEXTURE_2D, 1, &quad_tex_id);
    {
        constexpr int desired_channels_num{ 4 };
        const std::filesystem::path img_path("assets/textures/img.jpg");
        const auto str_img_path = img_path.string();

        int width       { 0 };
        int height      { 0 };
        int channels_num{ 0 };
        auto* img_ptr = stbi_load(
            str_img_path.c_str(), 
            &width, 
            &height, 
            &channels_num, 
            desired_channels_num
        );

        if (img_ptr == nullptr) {
            spdlog::error("Failed to load img from {}", str_img_path.c_str());
            return 1;
        }

        const auto num_levels = static_cast<int>(std::ceil(std::log2(std::min(
            static_cast<float>(width), 
            static_cast<float>(height)
        ))));
        glTextureStorage2D(quad_tex_id, num_levels, GL_RGBA8, width, height);
        glTextureParameteri(quad_tex_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(quad_tex_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(quad_tex_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTextureParameteri(quad_tex_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureSubImage2D(
            quad_tex_id,
            0, 0, 0, width, height,
            GL_RGBA, GL_UNSIGNED_BYTE,
            static_cast<const void*>(img_ptr)
        );
        glGenerateTextureMipmap(quad_tex_id);
        glBindTextureUnit(SHCONFIG_2D_TEX_BINDING, quad_tex_id);

        stbi_image_free(img_ptr);
        img_ptr = nullptr;
    }
    // bind
    basic_shader.bind();
    glBindVertexArray(quad_vao_id);
    // main loop
    while (!window.shouldClose()) {
        window.pollEvents();

        const auto[width, height] = window.size();
        glViewport(0, 0, width, height);
        glClearColor(
            clear_color[0], 
            clear_color[1],
            clear_color[2],
            1.F
        );

        ubo_data.aspect_ratio = 
            static_cast<float>(width) /
            static_cast<float>(height);
        
        glNamedBufferSubData(
            quad_ubo_id, 
            0, sizeof(ubo_data), 
            static_cast<const void*>(&ubo_data)
        );

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, quad_vertices.size()/4);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Config");
            ImGui::SliderFloat("scaling", &ubo_data.quad_scale, 0.F, 1.F);
            ImGui::Separator();
            ImGui::ColorPicker3("clear color", clear_color.data());
        ImGui::End();

        ImGui::Render();
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.swapBuffers();
    }
    // opengl stuff
    std::array<std::uint32_t, 2> buffers {{quad_vbo_id, quad_ubo_id}};
    glDeleteBuffers(buffers.size(), buffers.data());
    glDeleteVertexArrays(1, &quad_vao_id);
    glDeleteTextures(1, &quad_tex_id);
    basic_shader.deinit();
    // imgui stuff
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // window
    window.deinit();
}