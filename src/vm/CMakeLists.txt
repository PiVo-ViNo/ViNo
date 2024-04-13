cmake_minimum_required(VERSION 3.25)
project(
    ViNo_ViMa 
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

if(PROJECT_IS_TOP_LEVEL)
	find_package(vinogui REQUIRED)
endif()

if(WIN32)
    set(CMAKE_LINKER lld-link)
endif()

if(WIN32)
    add_executable(Vinovima_exe WIN32)
    add_link_options(-Wl,"/entry:mainCRTStartup /subsystem:windows")
else()
    add_executable(Vinovima_exe)
endif()

add_executable(Vinovima::exe ALIAS Vinovima_exe)

add_default_target_compile_definitions(Vinovima_exe)
set_property(TARGET Vinovima_exe PROPERTY OUTPUT_NAME Vinovima)

target_sources(Vinovima_exe PRIVATE
    VMMain.cpp
)
target_include_directories(Vinovima_exe PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
    "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/gui/external>"
    "$<BUILD_INTERFACE:${DEPS_SRC}/glm>"
    "$<BUILD_INTERFACE:${DEPS_SRC}/glfw3/include>"
    "$<BUILD_INTERFACE:${DEPS_SRC}/ft2/include>"
)

target_link_libraries(Vinovima_exe PUBLIC vinogui)

add_folders(vm)