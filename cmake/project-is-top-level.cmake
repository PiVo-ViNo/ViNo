# Эта переменная устанавливается project() в CMake 3.21+
string(
    COMPARE EQUAL
    "${CMAKE_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}" 
    PROJECT_IS_TOP_LEVEL)
