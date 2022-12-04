#version 450 core

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_texcoord;

layout(std140, binding = 5) uniform Transform {
    float scale;
    float aspect_ratio;
};

layout(location = 0) out vec2 out_texcoord;

void main() { 
    out_texcoord = in_texcoord;

    gl_Position = vec4(scale*in_position.x, aspect_ratio*scale*in_position.y, 0.0, 1.0);
}