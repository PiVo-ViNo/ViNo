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
	find_package(ViNo_lib REQUIRED)
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
add_executable(ViNo-Parser_test Parser/Parser_test.cpp)

target_include_directories(
	ViNo-Parser_test
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
	PUBLIC "$<BUILD_INTERFACE:${boost.ut_SOURCE_DIR}/include>"
)

target_compile_features(ViNo-Parser_test PRIVATE cxx_std_20)

add_default_target_compile_definitions(ViNo-Parser_test)

target_link_libraries(
	ViNo-Parser_test
	PRIVATE ViNo_lib
)

add_test(NAME ViNo-Parser_test COMMAND ViNo-Parser_test)

# ---- Token Scanner test ----
add_executable(ViNo-Token-Scanner_test TokenScanner/TokenScanner_test.cpp)

target_include_directories(
	ViNo-Token-Scanner_test
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
	PUBLIC "$<BUILD_INTERFACE:${boost.ut_SOURCE_DIR}/include>"
)

target_compile_features(ViNo-Token-Scanner_test PRIVATE cxx_std_20)

add_default_target_compile_definitions(ViNo-Token-Scanner_test)

target_link_libraries(
	ViNo-Token-Scanner_test
	PRIVATE ViNo_lib
)

add_test(NAME ViNo-Token-Scanner_test COMMAND ViNo-Token-Scanner_test)
add_folders(Test)