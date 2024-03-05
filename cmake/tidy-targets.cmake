set(TIDY_COMMAND clang-tidy CACHE STRING "Tidy check to use")

add_custom_target(
    tidy-check
    COMMAND "${CMAKE_COMMAND}"
    -D "TIDY_COMMAND=${TIDY_COMMAND}"
    -P "${PROJECT_SOURCE_DIR}/cmake/tidy.cmake"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    COMMENT "Checking typical programming errors."
    VERBATIM
)

add_custom_target(
    tidy-fix
    COMMAND "${CMAKE_COMMAND}"
    -D "TIDY_COMMAND=${TIDY_COMMAND}"
    -D FIX=YES
    -P "${PROJECT_SOURCE_DIR}/cmake/tidy.cmake"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    COMMENT "Fixing typical programming errors."
    VERBATIM
)