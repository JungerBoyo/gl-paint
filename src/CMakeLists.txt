# find_package(glfw3 REQUIRED)
# find_package(glad REQUIRED)
# find_package(imgui REQUIRED)
find_package(spdlog REQUIRED)

add_subdirectory(utils)
add_subdirectory(gl_layer)
add_subdirectory(ui_layer)
add_subdirectory(window_layer)

add_library(app_IMPL STATIC App.cpp)
target_link_libraries(app_IMPL PUBLIC app_INC)
target_link_libraries(app_IMPL
  PRIVATE 
    SHCONFIG
    window_layer_IMPL
    gl_layer_IMPL
    ui_layer_IMPL
)
target_link_system_libraries(app_IMPL
  PRIVATE 
    spdlog::spdlog
)
add_executable(${PROJECT_NAME} 
  main.cpp
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE 
    project_options
    project_warnings
    app_IMPL
)
target_link_system_libraries(${PROJECT_NAME}
  PRIVATE
    spdlog::spdlog
)

add_dependencies(${PROJECT_NAME} compile_and_copy_shaders_dirs copy_assets)

# target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_BINARY_DIR}/config")
