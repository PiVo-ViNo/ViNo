project(
	ViNoTests
	VERSION 0.0.1
	DESCRIPTION "Tests for Vi[sual] No[vel] Creator"
	HOMEPAGE_URL "https://github.com/SibJusha/ViNo"
	LANGUAGES CXX
)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

if(PROJECT_IS_TOP_LEVEL)
	find_package(ViNoCompiler_lib REQUIRED)
	enable_testing()
endif()

include(FetchContent)

set(BOOST.UT_VERSION_DOWNLOAD "2.0.1")
FetchContent_Declare(
	boost.ut
	GIT_REPOSITORY https://github.com/boost-ext/ut.git
	GIT_TAG v${BOOST.UT_VERSION_DOWNLOAD}
	SOURCE_DIR ${CMAKE_SOURCE_DIR}/_deps_src/boost.ut-${BOOST.UT_VERSION_DOWNLOAD}
)
FetchContent_Populate(boost.ut)

# ---- Parser test ----
add_executable(ViNo-Parser_test Compiler/Parser_test.cpp)

target_include_directories(
	ViNo-Parser_test
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/compiler>"
	PUBLIC "$<BUILD_INTERFACE:${boost.ut_SOURCE_DIR}/include>"
)

target_compile_features(ViNo-Parser_test PRIVATE cxx_std_20)

add_default_target_compile_definitions(ViNo-Parser_test)

target_link_libraries(
	ViNo-Parser_test
	PRIVATE ViNoCompiler_lib
)

add_test(NAME ViNo-Parser_test COMMAND ViNo-Parser_test)

# ---- Token Scanner test ----
add_executable(ViNo-Token-Scanner_test Compiler/TokenScanner_test.cpp)

target_include_directories(
	ViNo-Token-Scanner_test
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/compiler>"
	PUBLIC "$<BUILD_INTERFACE:${boost.ut_SOURCE_DIR}/include>"
)

target_compile_features(ViNo-Token-Scanner_test PRIVATE cxx_std_20)

add_default_target_compile_definitions(ViNo-Token-Scanner_test)

target_link_libraries(
	ViNo-Token-Scanner_test
	PRIVATE ViNoCompiler_lib
)

add_test(NAME ViNo-Token-Scanner_test COMMAND ViNo-Token-Scanner_test)

# ---- Full Compiler test ----
add_executable(ViNo-FullCompiler_test Compiler/FullCompiler_test.cpp)

target_include_directories(
	ViNo-FullCompiler_test
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/compiler>"
	PUBLIC "$<BUILD_INTERFACE:${boost.ut_SOURCE_DIR}/include>"
)

target_compile_features(ViNo-FullCompiler_test PRIVATE cxx_std_20)

add_default_target_compile_definitions(ViNo-FullCompiler_test)

target_link_libraries(
	ViNo-FullCompiler_test
	PRIVATE ViNoCompiler_lib
)

add_test(NAME ViNo-FullCompiler_test 
	COMMAND ViNo-FullCompiler_test
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Compiler
)

add_folders(Test)