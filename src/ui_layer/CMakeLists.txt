find_package(imgui REQUIRED)
find_package(glad REQUIRED)

add_library(ui_layer_IMPL STATIC UILayer.cpp)

target_link_libraries(ui_layer_IMPL PUBLIC ui_layer_INC)
target_include_directories(ui_layer_IMPL PRIVATE "${CMAKE_BINARY_DIR}/config")
target_link_system_libraries(ui_layer_IMPL
  PRIVATE
    imgui_bindings
    imgui::imgui
    glad::glad
)