add_library(events_INC INTERFACE Event.hpp)
target_include_directories(events_INC INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(events_INC INTERFACE common_INC)