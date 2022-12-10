#version 450 core

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_texcoord;

layout(std140, binding = 2) uniform GlobalMetadata {
    vec2 viewport_scale;
    float aspect_ratio;
};

struct InstanceTranslation {
    uint texture_index;
    uint transform_index;
};

struct ImageTransform {
    vec2 offset;
    vec2 scale;
    mat2 rotation;
};

layout(std140, binding = 3) uniform ImageMetadata {
    ImageTransform image_transforms[32];
    InstanceTranslation instance_translations[32];
};

layout(location = 0) out vec2 out_texcoord;
layout(location = 1) flat out uint out_texture_index;

void main() { 
    InstanceTranslation instance_translation = instance_translations[gl_InstanceIndex];
    ImageTransform image_transform = image_transforms[instance_translation.transform_index];

    out_texcoord = vec2(0.5, 0.5) + image_transform.offset + image_transform.scale * (
        image_transform.rotation * (
            in_texcoord + vec2(-0.5, -0.5)
        )
    );
    out_texture_index = instance_translation.texture_index & 0x1F;

    gl_Position = vec4(
        viewport_scale.x * in_position.x, 
        aspect_ratio * viewport_scale.y * in_position.y, 
        0.0, 
        1.0
    );
}