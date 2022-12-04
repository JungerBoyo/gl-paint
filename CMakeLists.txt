cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

### Add project options ###
include(FetchContent)
FetchContent_Declare(_project_options URL https://github.com/aminya/project_options/archive/refs/tags/v0.22.4.zip)
FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)
###########################

### set up project ###
project(template
  VERSION 1.0.0
  DESCRIPTION "C++ glfw3+opengl+imgui template project"
  HOMEPAGE_URL "https://github.com/JungerBoyo/opengl-template-project"
  LANGUAGES C CXX
)
set(GIT_SHA "Unknown" CACHE STRING "SHA of this build")
set(SUBSTRING "${GIT_SHA}" 0 8 GIT_SHORTENED_SHA)
#######################

### set up conan ###
#run_conan()
####################

### set up project options ###
option(DEV_MODE "<'^'>" OFF)
  set(ENABLE_CLANG_TIDY                   OFF)
  set(ENABLE_SANITIZER_ADDRESS            OFF)
  set(ENABLE_SANITIZER_UNDEFINED_BEHAVIOR OFF)
  set(ENABLE_COVERAGE                     OFF)
  set(WARNINGS_AS_ERRORS                  OFF)

if(DEV_MODE)
  set(ENABLE_CLANG_TIDY "ENABLE_CLANG_TIDY")
  set(ENABLE_COVERAGE "ENABLE_COVERAGE")

  if(NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    set(ENABLE_SANITIZER_ADDRESS "ENABLE_SANITIZER_ADDRESS")
    set(ENABLE_SANITIZER_UNDEFINED_BEHAVIOR "ENABLE_SANITIZER_UNDEFINED_BEHAVIOR")
  else()
    string(FIND "$ENV{PATH}" "${VSINSTALLDIR}" index_of_vs_install_dir)
    if(MSVC AND "${index_of_vs_install_dir}" STREQUAL "-1")
      set(ENABLE_SANITIZER_ADDRESS "ENABLE_SANITIZER_ADDRESS")
    endif()
  endif()
endif()

project_options(
  ENABLE_CACHE
  ${ENABLE_CLANG_TIDY}
  ENABLE_VS_ANALYSIS
  ENABLE_CONAN
  ${ENABLE_COVERAGE}
  ${ENABLE_SANITIZER_ADDRESS}
  ${ENABLE_SANITIZER_UNDEFINED_BEHAVIOR}
  ${WARNINGS_AS_ERRORS}
)
##############################

### targets ###
add_subdirectory(shaders)
add_subdirectory(assets)
add_subdirectory(config)
add_subdirectory(res)
add_subdirectory(include)
add_subdirectory(src)
###############

### MSVC specific ###
if(MSVC)
  set(all_targets)
  get_all_targets(all_targets ${CMAKE_CURRENT_SOURCE_DIR})
  set_target_properties(${all_targets} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=$(VC_ExecutablePath_x64);%PATH%")
endif()
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
#####################


### package project ###
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "./")
install(FILES ${PROJECT_SOURCE_DIR}/imgui.ini DESTINATION "./")
install(DIRECTORY ${PROJECT_SOURCE_DIR}/shaders/bin DESTINATION "shaders/")
install(DIRECTORY ${PROJECT_SOURCE_DIR}/shaders/src DESTINATION "shaders/")
install(DIRECTORY ${PROJECT_SOURCE_DIR}/assets/textures DESTINATION "assets/")

set(CPACK_PACKAGE_FILE_NAME 
  "${PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_VERSION}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
)
include(CPack)
#######################