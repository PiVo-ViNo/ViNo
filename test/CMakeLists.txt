project(
	ViNoTests
	VERSION 0.0.1
	DESCRIPTION "Tests for Vi[sual] No[vel] Creator"
	HOMEPAGE_URL "https://github.com/SibJusha/ViNo"
	LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

if(PROJECT_IS_TOP_LEVEL)
	find_package(ViNo_lib REQUIRED)
	enable_testing()
endif()

# ---- Parser test ----
add_executable(ViNo-Parser_test Parser/cmake-parser-cpp_test.cpp)

target_compile_features(ViNo-Parser_test PRIVATE cxx_std_20)
target_compile_options(ViNo-Parser_test PRIVATE -O)

add_default_target_compile_definitions(ViNo-Parser_test)

target_link_libraries(
	ViNo-Parser_test
	PRIVATE ViNo_lib)

add_test(NAME ViNo-Parser_test COMMAND ViNo-Parser_test)

# ---- Token Scanner test ----
add_executable(ViNo-Token-Scanner_test TokenScanner/cmake-token-scanner-cpp_test.cpp)

target_compile_features(ViNo-Token-Scanner_test PRIVATE cxx_std_20)
target_compile_options(ViNo-Token-Scanner_test PRIVATE -O)

add_default_target_compile_definitions(ViNo-Token-Scanner_test)

target_link_libraries(
	ViNo-Token-Scanner_test
	PRIVATE ViNo_lib)

add_test(NAME ViNo-Token-Scanner_test COMMAND ViNo-Token-Scanner_test)
add_folders(Test)