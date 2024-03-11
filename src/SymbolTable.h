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

//! Paths as strings could be changed to filesystem::path in future
/**
    @file Symbol Table simple implementaion with hash table
*/

#pragma once

#include <deque>
#include <string>
#include <unordered_map>

#include "Persona.h"

namespace vino {

/**
    @brief Chain Environment of Symbol Tables (stack)
    @details
*/
class SymbolTableEnv {
public:
    using SymbolTable = std::unordered_map<const std::string, Persona>;

    SymbolTableEnv() : _chain_env() {}

    SymbolTableEnv(const SymbolTableEnv& other) : _chain_env(other._chain_env)
    {
    }

    /// Check if there is persona with such an id in Env
    bool exists(const std::string&) noexcept;

    /**
    @brief Add persona to Env
    @details Creates new link in Env chain if such persona already exists
    @return Reference of created persona
    */
    Persona& add_persona(const std::string&) noexcept;

    /**
    @brief Add member/change in persona
    @details Creates new link in Env chain if such member already exists
    @return `false` if there is no such persona in Env
    */
    bool add_info_to(const std::string& persona_id,
                     const std::string& member_id,
                     const std::string& member_parameter) noexcept;

    /// @throw runtime_error if there is no such Persona
    Persona& get_persona(const std::string& persona_id);
    
private:
    std::deque<SymbolTable> _chain_env;
};

}  // namespace vino
