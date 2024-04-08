cmake_minimum_required(VERSION 3.25)
project(
    ViNo_VM
    VERSION 0.1.0
    DESCRIPTION "Vi[sual] No[vel] Creator: VM"
    HOMEPAGE_URL "https://github.com/PiVo-ViNo/ViNo"
    LANGUAGES CXX
)

set(DEPS_SRC "${CMAKE_SOURCE_DIR}/_deps_src")
include(${CMAKE_SOURCE_DIR}/cmake/project-is-top-level.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/folders.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/variables.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/add_default_target_compile_definitions.cmake)

if(WIN32)
    set(CMAKE_LINKER lld-link)
endif()

if(WIN32)
    add_executable(ViNoVM_exe WIN32)
    add_link_options(-Wl,"/entry:mainCRTStartup /subsystem:windows")
else()
    add_executable(ViNoVM_exe)
endif()

add_executable(ViNoVM::exe ALIAS ViNoVM_exe)
add_default_target_compile_definitions(ViNoVM_exe)
target_sources(ViNoVM_exe PRIVATE
    VMMain.cpp
)
target_include_directories(ViNoVM_exe PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
    "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/gui/external>"
    "$<BUILD_INTERFACE:${DEPS_SRC}/glm>"
    "$<BUILD_INTERFACE:${DEPS_SRC}/glfw3/include>"
    "$<BUILD_INTERFACE:${DEPS_SRC}/ft2/include>"
)

target_link_libraries(ViNoVM_exe PUBLIC vinogui)

add_folders(vm)