# ---- Минимально необходимая версия CMake ----
cmake_minimum_required(VERSION 3.20)

# ---- Проверяем, что сборка пойдет не в каталог проекта ----
include(cmake/prelude.cmake)

project(
	ViNo
	VERSION 0.0.1
	DESCRIPTION "Vi[sual] No[vel] Creator"
	HOMEPAGE_URL "https://github.com/SibJusha/ViNo"
	LANGUAGES CXX)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Добавляем вспомогательную cmake-функцию для добавления определений таргета ----
include(cmake/add_default_target_compile_definitions.cmake)

# ---- Объявлям библиотеку основного функционала ----
add_library(ViNo_lib OBJECT src/Parser.cpp src/TokenScanner.cpp src/TokenEnum.cpp)

target_include_directories(
	ViNo_lib ${warning_guard} PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
	"$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>")

target_compile_features(ViNo_lib PUBLIC cxx_std_20)
target_compile_options(ViNo_lib PRIVATE -O)

add_default_target_compile_definitions(ViNo_lib)

# ---- Объявляем исполняемый файл ----
add_executable(ViNo_exe src/main.cpp)
add_executable(ViNo::exe ALIAS ViNo_exe)

add_default_target_compile_definitions(ViNo_exe)

set_property(TARGET ViNo_exe PROPERTY OUTPUT_NAME ViNo)

target_compile_features(ViNo_exe PRIVATE cxx_std_20)
target_compile_options(ViNo_exe PRIVATE -O)

target_link_libraries(
	ViNo_exe
	PRIVATE ViNo_lib)

# ---- Правила установки ----
if(NOT CMAKE_SKIP_INSTALL_RULES)
	set(CPACK_PACKAGE_NAME "ViNo")
	include(cmake/install-rules.cmake)
endif()

# ---- Режим разработчика ----
if(NOT ViNo_DEVELOPER_MODE)
	return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
	message(AUTHOR_WARNING "Режим разработчика предназначен для разработчиков ViNo.")
endif()

include(cmake/dev-mode.cmake)