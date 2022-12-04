[![ci](https://github.com/JungerBoyo/opengl-template-project/actions/workflows/ci.yml/badge.svg)](https://github.com/JungerBoyo/opengl-template-project/actions/workflows/ci.yml)

# opengl template project
C++ OpenGL template project set up with general structure, UI library (ImGui), img loading library (Stb), shaders compilation to spirv and CI 

## Prerequisites
    - conan package manager
    - c++ compiler
    - cmake
    - glslangValidator + spirv-opt (optional), to disable add -DREMOTE_BUILD=ON flag to cmake configure
    - OpenGL (>= 4.5)
    - ninja (optional)
    - ccache
    - clangtidy (>= 14.0.0)
