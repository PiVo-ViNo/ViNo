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
add_library(vino-compiler_lib OBJECT 
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
	vino-compiler_lib ${warning_guard} PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
)

target_compile_features(vino-compiler_lib PUBLIC cxx_std_20)

add_default_target_compile_definitions(vino-compiler_lib)

# ---- Declare executable ----
add_executable(vino-compiler_exe main.cpp)
add_executable(vino-compiler::exe ALIAS vino-compiler_exe)

add_default_target_compile_definitions(vino-compiler_exe)

set_property(TARGET vino-compiler_exe PROPERTY OUTPUT_NAME vino-compiler)

target_compile_features(vino-compiler_exe PRIVATE cxx_std_20)

target_link_libraries(
	vino-compiler_exe
	PRIVATE vino-compiler_lib
)

add_folders(compiler)