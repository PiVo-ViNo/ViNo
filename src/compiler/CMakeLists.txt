cmake_minimum_required(VERSION 3.25)
project(
	ViNo_Compiler
	VERSION 0.1.0
	DESCRIPTION "Vi[sual] No[vel] Creator: Compiler"
	HOMEPAGE_URL "https://github.com/PiVo-ViNo/ViNo"
	LANGUAGES CXX
)

include(${CMAKE_SOURCE_DIR}/cmake/folders.cmake)
# ---- Additional cmake function to add target definitions ----
include(${CMAKE_SOURCE_DIR}/cmake/add_default_target_compile_definitions.cmake)

# ---- The core library ----
add_library(ViNoCompiler_lib OBJECT 
	Parser.cpp
	Persona.cpp
	SemanticAnalyzer.cpp
	SymbolTable.cpp
	TokenEnum.cpp
	TokenScanner.cpp
	VisitorImplementations.cpp
	MainCompiler.cpp
	CodeGen.cpp
)

target_include_directories(
	ViNoCompiler_lib ${warning_guard} PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
)

target_compile_features(ViNoCompiler_lib PUBLIC cxx_std_20)

add_default_target_compile_definitions(ViNoCompiler_lib)

# ---- Declare executable ----
add_executable(ViNoCompiler_exe main.cpp)
add_executable(ViNoCompiler::exe ALIAS ViNoCompiler_exe)

add_default_target_compile_definitions(ViNoCompiler_exe)

set_property(TARGET ViNoCompiler_exe PROPERTY OUTPUT_NAME ViNoCompiler)

target_compile_features(ViNoCompiler_exe PRIVATE cxx_std_20)

target_link_libraries(
	ViNoCompiler_exe
	PRIVATE ViNoCompiler_lib
)

add_folders(Compiler)