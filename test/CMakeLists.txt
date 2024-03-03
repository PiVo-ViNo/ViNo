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

include(FetchContent)

set(BOOST.UT_VERSION_DOWNLOAD "2.0.0")

# dirty hacks, redo with download header from boost.ut git
set(CPM_PackageProject.cmake_SOURCE "")
set(CPM_DECLARATION_PackageProject.cmake "")

FetchContent_Declare(
	boost.ut
	GIT_REPOSITORY https://github.com/boost-ext/ut.git
	GIT_TAG v${BOOST.UT_VERSION_DOWNLOAD}
	SOURCE_DIR ${CMAKE_SOURCE_DIR}/_deps_src/boost.ut-${BOOST.UT_VERSION_DOWNLOAD})

set(CPM_USE_LOCAL_PACKAGES ON)
set(CPM_LOCAL_PACKAGES_ONLY OFF)
set(CPM_DOWNLOAD_ALL OFF)
set(CPM_DONT_UPDATE_MODULE_PATH ON)
set(CPM_DONT_CREATE_PACKAGE_LOCK ON)
set(CPM_INCLUDE_ALL_IN_PACKAGE_LOCK ON)
set(CPM_USE_NAMED_CACHE_DIRECTORIES ON)

FetchContent_MakeAvailable(boost.ut)

# ---- Parser test ----
add_executable(ViNo-Parser_test Parser/Parser_test.cpp)

target_compile_features(ViNo-Parser_test PRIVATE cxx_std_20)
target_compile_options(ViNo-Parser_test PRIVATE -O)

add_default_target_compile_definitions(ViNo-Parser_test)

target_link_libraries(
	ViNo-Parser_test
	PRIVATE ViNo_lib
	PRIVATE Boost::ut)

add_test(NAME ViNo-Parser_test COMMAND ViNo-Parser_test)

# ---- Token Scanner test ----
add_executable(ViNo-Token-Scanner_test TokenScanner/TokenScanner_test.cpp)

target_compile_features(ViNo-Token-Scanner_test PRIVATE cxx_std_20)
target_compile_options(ViNo-Token-Scanner_test PRIVATE -O)

add_default_target_compile_definitions(ViNo-Token-Scanner_test)

target_link_libraries(
	ViNo-Token-Scanner_test
	PRIVATE ViNo_lib
	PRIVATE Boost::ut)

add_test(NAME ViNo-Token-Scanner_test COMMAND ViNo-Token-Scanner_test)
add_folders(Test)