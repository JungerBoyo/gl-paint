#version 450 core

layout(location = 0) out vec4 fragment;

layout(binding = 3) uniform sampler2D u_textures[32];

layout(location = 0) in vec2 in_texcoord;
layout(location = 1) flat in uint in_texture_index;

void main() {
    vec4 texture_component = vec4(0.0);
    switch (in_texture_index) {
        case 0:  texture_component = texture(u_textures[0],  in_texcoord); break;
        case 1:  texture_component = texture(u_textures[1],  in_texcoord); break;
        case 2:  texture_component = texture(u_textures[2],  in_texcoord); break;
        case 3:  texture_component = texture(u_textures[3],  in_texcoord); break;
        case 4:  texture_component = texture(u_textures[4],  in_texcoord); break;
        case 5:  texture_component = texture(u_textures[5],  in_texcoord); break;
        case 6:  texture_component = texture(u_textures[6],  in_texcoord); break;
        case 7:  texture_component = texture(u_textures[7],  in_texcoord); break;
        case 8:  texture_component = texture(u_textures[8],  in_texcoord); break;
        case 9:  texture_component = texture(u_textures[9],  in_texcoord); break;
        case 10: texture_component = texture(u_textures[10], in_texcoord); break;
        case 11: texture_component = texture(u_textures[11], in_texcoord); break;
        case 12: texture_component = texture(u_textures[12], in_texcoord); break;
        case 13: texture_component = texture(u_textures[13], in_texcoord); break;
        case 14: texture_component = texture(u_textures[14], in_texcoord); break;
        case 15: texture_component = texture(u_textures[15], in_texcoord); break;
        case 16: texture_component = texture(u_textures[16], in_texcoord); break;
        case 17: texture_component = texture(u_textures[17], in_texcoord); break;
        case 18: texture_component = texture(u_textures[18], in_texcoord); break;
        case 19: texture_component = texture(u_textures[19], in_texcoord); break;
        case 20: texture_component = texture(u_textures[20], in_texcoord); break;
        case 21: texture_component = texture(u_textures[21], in_texcoord); break;
        case 22: texture_component = texture(u_textures[22], in_texcoord); break;
        case 23: texture_component = texture(u_textures[23], in_texcoord); break;
        case 24: texture_component = texture(u_textures[24], in_texcoord); break;
        case 25: texture_component = texture(u_textures[25], in_texcoord); break;
        case 26: texture_component = texture(u_textures[26], in_texcoord); break;
        case 27: texture_component = texture(u_textures[27], in_texcoord); break;
        case 28: texture_component = texture(u_textures[28], in_texcoord); break;
        case 29: texture_component = texture(u_textures[29], in_texcoord); break;
        case 30: texture_component = texture(u_textures[30], in_texcoord); break;
        case 31: texture_component = texture(u_textures[31], in_texcoord); break;
    }
    fragment = texture_component;
}