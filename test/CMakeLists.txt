cmake_minimum_required(VERSION 3.12)
project(ViNo_base VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

add_executable(tokenscanner
            ../src/TokenScanner.cpp
            test_TokenScanner.cpp)

include_directories(../include
                    ../src)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
