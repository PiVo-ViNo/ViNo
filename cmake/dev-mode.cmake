include(cmake/folders.cmake)
include(CTest)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()

option(BUILD_DOCS "Build documentation using Doxygen" OFF)
if(BUILD_DOCS)
  include(cmake/docs.cmake)
endif()

include(cmake/lint-targets.cmake)
include(cmake/spell-targets.cmake)
include(cmake/tidy-targets.cmake)

add_folders(Project)