find_package(glad REQUIRED)
find_package(spdlog REQUIRED)

add_library(window_layer_IMPL STATIC Window.cpp WindowLayer.cpp)

target_link_libraries(window_layer_IMPL PUBLIC window_layer_INC)
target_include_directories(window_layer_IMPL PRIVATE "${CMAKE_BINARY_DIR}/config")
target_link_system_libraries(window_layer_IMPL
  PRIVATE
    SHCONFIG
    glad::glad
    spdlog::spdlog
)
