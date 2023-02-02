#version 450 core

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_texcoord;

struct InstanceToTextureTranslation {
    uint texture_layer_index;
    float alpha;
};
layout(std140, binding = 5) uniform WorkspaceData {
    vec2 offset;
    float aspect_ratio;
    float scale;
    InstanceToTextureTranslation instance_to_texture_translations[128];
};

layout(location = 0) out vec2 out_texcoord;
layout(location = 1) out flat float out_texid;
// layout(location = 2) out flat float out_alpha;

void main() { 
    out_texcoord = in_texcoord;

    InstanceToTextureTranslation instance_to_texture_translation =
        instance_to_texture_translations[gl_InstanceIndex];

    out_texid = float(
        instance_to_texture_translation.texture_layer_index
    );
    // out_alpha = instance_to_texture_translation.alpha;

    gl_Position = vec4(
        (scale * in_position.x) + offset.x, 
        (scale * in_position.y) + offset.y, 
        0.0, 
        1.0
    );
}