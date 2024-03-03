cmake_minimum_required(VERSION 3.14)

macro(default name)
  if(NOT DEFINED "${name}")
    set("${name}" "${ARGN}")
  endif()
endmacro()

default(TIDY_COMMAND clang-tidy)
default(FIX NO)

set(flag "")
if(FIX)
  set(flag --fix-errors)
endif()

execute_process(
  COMMAND ${TIDY_COMMAND}
  --config-file=./.clang-tidy ${flag}
  "./include/custom_errors.h"
  "./include/stuff.h"
  "./include/TokenEnum.h"
  "./src/main.cpp"
  "./src/Parser.cpp"
  "./src/Parser.h"
  "./src/TokenScanner.cpp"
  "./src/TokenScanner.h"
  "./test/Parser/cmake-parser-cpp_test.cpp"
  "./test/TokenScanner/cmake-token-scanner-cpp_test.cpp"
)