add_library(utils_INC INTERFACE RingQueue.hpp RingQueue.tpp image_ops.hpp)
target_include_directories(utils_INC INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(utils_INC INTERFACE common_INC)