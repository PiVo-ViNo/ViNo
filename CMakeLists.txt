# ---- The minimum required version of CMake ----
cmake_minimum_required(VERSION 3.25)

# ---- Check that the build will not go to the project source directory ----
include(cmake/prelude.cmake)

set(CMAKE_CXX_STANDARD_REQUIRED 20)

project(
	ViNo
	VERSION 0.1.0
	DESCRIPTION "Vi[sual] No[vel] Creator"
	HOMEPAGE_URL "https://github.com/PiVo-ViNo/ViNo"
	LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Additional cmake function to add target definitions ----
include(cmake/add_default_target_compile_definitions.cmake)

# ---- Build All Components of ViNo ----
add_subdirectory(src/compiler)
add_subdirectory(src/gui)
add_subdirectory(src/vm)

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