# ---- The minimum required version of CMake ----
cmake_minimum_required(VERSION 3.20)

# ---- Check that the build will not go to the project source directory ----
include(cmake/prelude.cmake)

set(CMAKE_CXX_STANDARD_REQUIRED 20)

project(
	ViNo
	VERSION 0.0.1
	DESCRIPTION "Vi[sual] No[vel] Creator"
	HOMEPAGE_URL "https://github.com/PiVo-ViNo/ViNo"
	LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Additional cmake function to add target definitions ----
include(cmake/add_default_target_compile_definitions.cmake)

# ---- The core library ----
add_library(ViNo_lib OBJECT 
	src/AST.cpp
	src/Parser.cpp
	src/Persona.cpp
	src/SemanticAnalyzer.cpp
	src/SymbolTable.cpp
	src/TokenEnum.cpp
	src/TokenScanner.cpp
)

target_include_directories(
	ViNo_lib ${warning_guard} PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
	"$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
)

target_compile_features(ViNo_lib PUBLIC cxx_std_20)

add_default_target_compile_definitions(ViNo_lib)

# ---- Declare executable ----
add_executable(ViNo_exe src/main.cpp)
add_executable(ViNo::exe ALIAS ViNo_exe)

add_default_target_compile_definitions(ViNo_exe)

set_property(TARGET ViNo_exe PROPERTY OUTPUT_NAME ViNo)

target_compile_features(ViNo_exe PRIVATE cxx_std_20)

target_link_libraries(
	ViNo_exe
	PRIVATE ViNo_lib
)

# ---- Install rules ----
if(NOT CMAKE_SKIP_INSTALL_RULES)
	set(CPACK_PACKAGE_NAME "ViNo")
	include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----
if(NOT ViNo_DEVELOPER_MODE)
	return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
	message(AUTHOR_WARNING "Developer mode is intended for developers of ViNo.")
endif()

include(cmake/dev-mode.cmake)