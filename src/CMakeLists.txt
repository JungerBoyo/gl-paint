find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(imgui REQUIRED)
find_package(stb REQUIRED)
find_package(spdlog REQUIRED)

add_library(boilerplate_IMPL STATIC Shader.cpp Window.cpp)
target_link_libraries(boilerplate_IMPL PUBLIC boilerplate_INC)

target_link_system_libraries(boilerplate_IMPL
  PRIVATE
    glad::glad
    spdlog::spdlog
    SHCONFIG
)

add_executable(${PROJECT_NAME} 
  main.cpp
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE 
    project_options
    project_warnings
    boilerplate_IMPL
    SHCONFIG
)

target_link_system_libraries(${PROJECT_NAME}
  PRIVATE
    imgui_bindings
    imgui::imgui
    spdlog::spdlog
    glad::glad
    stbi
)

add_dependencies(${PROJECT_NAME} compile_and_copy_shaders_dirs copy_assets)

target_include_directories(${PROJECT_NAME} 
  PRIVATE 
    "${CMAKE_BINARY_DIR}/config"
)
