///////////////////////////////////////////////////////////////////////////////
// SymbolTable.h                //    /^--^\     /^--^\     /^--^\           //
//                              //    \____/     \____/     \____/           //
// Created on Sat Mar 05 2024   //   /      \   /      \   /      \          //
//                              //  |        | |        | |        |         //
//                              //   \__  __/   \__  __/   \__  __/          //
//                              //|^|^|^\ \^|^|^|^/ /^|^|^|^|^\ \^|^|^|^|^|^|//
//                              //| | | |\ \| | |/ /| | | | | |\ \ | | | | | //
// (c) Andrjusha (aka SibJusha) //| | |  / / | | \ \| | | | | |/ /| | | | | |//
//                              //| | |  \/| | | |\/| | | | | |\/ | | | | | |//
///////////////////////////////////////////////////////////////////////////////

//! No scopes in this version, i.e. no same names
//   for personas or persona's members, but members of different personas can
//   have the same names
//! Paths as strings could be changed to filesystem::path in future
/**
    @file Symbol Table simple implementaion with hash table
*/

#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "Persona.h"

namespace vino {

/**
    @brief
    @details
*/
struct SymbolTable {
    std::unordered_set<Persona> _personas;
};

}  // namespace vino
