find_package(glad REQUIRED)

add_library(utils_IMPL STATIC image_ops.cpp)
target_link_libraries(utils_IMPL PUBLIC utils_INC)
target_link_system_libraries(utils_IMPL PRIVATE stbi glad::glad)