cmake_minimum_required(VERSION 3.25)
project(
    ViNo_gui 
    VERSION 0.1.0 
	DESCRIPTION "Vi[sual] No[vel] Creator: GUI Library"
	HOMEPAGE_URL "https://github.com/PiVo-ViNo/ViNo"
    LANGUAGES C CXX
)

if(WIN32)
    set(CMAKE_LINKER lld-link)
endif()

# set(BUILD_SHARED_LIBS ON)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/runtime")

set(DEPS_SRC "${CMAKE_SOURCE_DIR}/_deps_src")
include(FetchContent)
FetchContent_Declare(
    glm	
	URL https://github.com/g-truc/glm/releases/download/1.0.1/glm-1.0.1-light.7z 
	SOURCE_DIR ${DEPS_SRC}/glm
)
FetchContent_Declare(
    ft2
    GIT_REPOSITORY https://github.com/freetype/freetype.git
    GIT_SHALLOW 1
    SOURCE_DIR ${DEPS_SRC}/ft2
)
FetchContent_Declare(
    glfw3
    URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
    SOURCE_DIR ${DEPS_SRC}/glfw3
)

# ---- Disable (dev) mode while configuring external libraries ----
set(no_dev_warnings_backup "$CACHE{CMAKE_SUPPRESS_DEVELOPER_WARNINGS}")
set(no_error_dev_backup "$CACHE{CMAKE_SUPPRESS_DEVELOPER_ERRORS}")
set(build_type_backup "$CACHE{CMAKE_BUILD_TYPE}")

set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS ON CACHE INTERNAL "" FORCE)
set(CMAKE_SUPPRESS_DEVELOPER_ERRORS ON CACHE INTERNAL "" FORCE)
set(CMAKE_BUILD_TYPE Release CACHE INTERNAL "" FORCE)

FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
    FetchContent_Populate(glm)
    # add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
endif()

FetchContent_GetProperties(ft2)
if(NOT ft2_POPULATED)
    FetchContent_Populate(ft2)
    add_subdirectory(${ft2_SOURCE_DIR} ${ft2_BINARY_DIR})
endif()

FetchContent_GetProperties(glfw3)
if(NOT glfw3_POPULATED)
    if(WIN32)
        set(GLFW_BUILD_WIN32 ON)
    endif()
    set(GLFW_BUILD_TESTS OFF) 
    set(GLFW_BUILD_EXAMPLES OFF)
    set(GLFW_BUILD_DOCS OFF)
    FetchContent_Populate(glfw3)
    add_subdirectory(${glfw3_SOURCE_DIR} ${glfw3_BINARY_DIR})
endif()

# ---- Return (dev) mode ----
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS ${no_dev_warnings_backup} CACHE INTERNAL "" FORCE)
set(CMAKE_SUPPRESS_DEVELOPER_ERRORS ${no_error_dev_backup} CACHE INTERNAL "" FORCE)
set(CMAKE_BUILD_TYPE ${build_type_backup} CACHE INTERNAL "" FORCE)

# ---------------------------------------------------------

include(${CMAKE_SOURCE_DIR}/cmake/project-is-top-level.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/folders.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/variables.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/add_default_target_compile_definitions.cmake)
 
if(CMAKE_BUILD_TYPE EQUAL Release)
    add_compile_options(-O2)
endif()

# ---- Core GUI Library ----
add_library(vinogui STATIC 
    vinogui.cpp
    glad.c 
    Window.cpp
    ImgData.cpp
    Fonts.cpp
    Box.cpp
)

target_include_directories(
    vinogui ${warning_guard} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external>"
    "$<BUILD_INTERFACE:${DEPS_SRC}/ft2/include>"
    "$<BUILD_INTERFACE:${DEPS_SRC}/glm>"
    "$<BUILD_INTERFACE:${DEPS_SRC}/glfw3/include>"
)
add_default_target_compile_definitions(vinogui)

target_link_libraries(vinogui
    PUBLIC glfw 
    PUBLIC freetype
)
target_compile_features(vinogui PUBLIC cxx_std_20)

add_folders(gui)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

# ---- Developer mode only ----
if(NOT ViNo_DEVELOPER_MODE)
	return()
endif()

# Executable
if(WIN32)
    add_executable(TestViNoGUI_exe WIN32)
    add_link_options(-Wl,"/entry:mainCRTStartup /subsystem:windows")
else()
    add_executable(TestViNoGUI_exe)
endif()

add_executable(TestViNoGUI::exe ALIAS TestViNoGUI_exe)

target_sources(TestViNoGUI_exe PUBLIC
    test_app/main.cpp
    test_app/TitleScreen.cpp
    test_app/MainMenu.cpp
)
target_include_directories(
    TestViNoGUI_exe ${warning_guard} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<BUILD_INTERFACE:${DEPS_SRC}/ft2/include>"
    "$<BUILD_INTERFACE:${DEPS_SRC}/glm>"
    "$<BUILD_INTERFACE:${DEPS_SRC}/glfw3/include>"
)
add_default_target_compile_definitions(TestViNoGUI_exe)

set_property(TARGET TestViNoGUI_exe PROPERTY OUTPUT_NAME TestViNoGUI)

target_link_libraries(TestViNoGUI_exe PUBLIC vinogui)

file(COPY   "${CMAKE_SOURCE_DIR}/src/gui/test_app/res" 
            "${CMAKE_SOURCE_DIR}/src/gui/test_app/shaders" 
            "${CMAKE_SOURCE_DIR}/src/gui/test_app/fonts" 
    DESTINATION "${CMAKE_BINARY_DIR}/src/gui"
)

# set(BUILD_SHARED_LIBS OFF)