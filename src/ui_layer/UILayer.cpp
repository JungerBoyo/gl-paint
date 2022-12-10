#include <UILayer.hpp>



#include <imgui.h>
#include <imgui_bindings/imgui_impl_glfw.h>
#include <imgui_bindings/imgui_impl_opengl3.h>

#include <config.hpp>

using namespace glpaint;

UILayer::UILayer(void *window_handle, EventQueue &event_queue) : 
    event_queue_(event_queue) {
    // initialize imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(window_handle), true);
    ImGui_ImplOpenGL3_Init(cmake::glsl_version.data());
}

void UILayer::update(u32 viewport_texture_id) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // here goes UI
    // render viewport texture
    static constexpr auto image_window_flags { 
        ImGuiWindowFlags_NoMove     |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize   |
        ImGuiWindowFlags_NoTitleBar
    };
    ImGui::Begin("##viewport", nullptr, image_window_flags);
    ImGui::Image((void*)(intptr_t)viewport_texture_id, ImGui::GetWindowSize());// NOLINT, interfacing with imgui
    ImGui::End();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UILayer::deinit() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}