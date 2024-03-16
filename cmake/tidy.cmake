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
  "./src/AST.cpp"
  "./src/AST.h"
  "./src/custom_errors.h"
  "./src/main.cpp"
  "./src/Parser.cpp"
  "./src/Parser.h"
  "./src/Persona.cpp"
  "./src/Persona.h"
  "./src/SemanticAnalyzer.cpp"
  "./src/SemanticAnalyzer.h"
  "./src/stuff.h"
  "./src/SymbolTable.cpp"
  "./src/SymbolTable.h"
  "./src/TokenEnum.cpp"
  "./src/TokenEnum.h"
  "./src/TokenScanner.cpp"
  "./src/TokenScanner.h"
  "./test/Parser/Parser_test.cpp"
  "./test/TokenScanner/TokenScanner_test.cpp"
)