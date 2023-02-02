#version 450 core

layout(location = 0) out vec4 fragment;

layout(binding = 4) uniform sampler2DArray u_tex_array;

layout(location = 0) in vec2 in_texcoord;
layout(location = 1) in flat float in_texid;
// layout(location = 2) in flat float in_alpha;

void main() {
    vec4 tex_component = texture(u_tex_array, vec3(in_texcoord, in_texid));
    fragment = tex_component;
}