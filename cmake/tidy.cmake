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
  "./src/*"
)