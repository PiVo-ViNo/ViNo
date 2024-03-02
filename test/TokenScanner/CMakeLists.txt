cmake_minimum_required(VERSION 3.24)
project(cmake-token-scanner-cppTests VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

set(BASE_SRC ../..)

include(CTest)
enable_testing()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
#set(CMAKE_CXX_FLAGS_DEBUG "-g")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(cmake-token-scanner-cpp_test
  ${BASE_SRC}/src/TokenScanner.cpp
  cmake-token-scanner-cpp_test.cpp)

include_directories(${BASE_SRC}/include
  ${BASE_SRC}/src)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# Родительский проект не экспортирует свою целевую библиотеку, поэтому этот CML неявно зависит от добавления из него, т. е. тестирование выполняется только из дерева сборки и невозможно из места установки.

include(FetchContent)

set(BOOST.UT_VERSION_DOWNLOAD "2.0.0")
FetchContent_Declare(
	boost.ut
	GIT_REPOSITORY http://github.com/boost-ext/ut.git
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

# ---- Тест lib ----

target_compile_features(cmake-token-scanner-cpp_test PRIVATE cxx_std_20)
target_compile_options(cmake-token-scanner-cpp_test PRIVATE -g)

target_link_libraries(
	cmake-token-scanner-cpp_test
	PRIVATE Boost::ut)

add_test(NAME cmake-token-scanner-cpp_test COMMAND cmake-token-scanner-cpp_test)
#[===[
if(WIN32 AND MINGW)
	target_link_options(cmake-token-scanner-cpp_test PRIVATE -static -static-libgcc -static-libstdc++)
endif()
]===]