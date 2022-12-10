find_package(glfw3 REQUIRED)

add_library(window_layer_INC INTERFACE Window.hpp Window.tpp WindowLayer.hpp)

target_link_system_libraries(window_layer_INC INTERFACE glfw::glfw)
target_include_directories(window_layer_INC INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(window_layer_INC INTERFACE common_INC utils_INC events_INC)