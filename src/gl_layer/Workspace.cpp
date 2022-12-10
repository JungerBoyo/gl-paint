#include <Workspace.hpp>

#include <image_ops.hpp>

#include <glad/glad.h>

using namespace glpaint;

Workspace::Workspace(i32 workspace_w, i32 workspace_h) :
    workspace_tex2d_array_({
        workspace_w,
        workspace_h,
        INITIAL_CAPACITY,
        GL_RGBA32F,
        GL_RGBA,
        GL_FLOAT
    }),
    instance_to_texture_translations_({0, 1}) {

    const auto img = loadImageU8("assets/textures/transparent50.png", 4);
    workspace_tex2d_array_.loadData(
        BASE_TEX2D_INDEX, 
        static_cast<const void*>(img->data.data())
    );
}
void Workspace::update() {

}
void Workspace::deinit() {
    workspace_tex2d_array_.deinit();
}