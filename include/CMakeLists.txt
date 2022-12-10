add_library(common_INC INTERFACE common.hpp)
target_include_directories(common_INC INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(utils)
add_subdirectory(events)
add_subdirectory(gl_layer)
add_subdirectory(ui_layer)
add_subdirectory(window_layer)

add_library(app_INC INTERFACE App.hpp)
target_include_directories(app_INC INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(app_INC 
  INTERFACE 
    window_layer_INC
    gl_layer_INC
    ui_layer_INC
    events_INC
    common_INC
)