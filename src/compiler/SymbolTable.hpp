///////////////////////////////////////////////////////////////////////////////
// SymbolTable.hpp              //    /^--^\     /^--^\     /^--^\           //
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
    @file Symbol Table simple implementation with hash table
*/

#pragma once

#include "Persona.hpp"

#include <deque>
#include <string>
#include <unordered_map>

namespace vino {

/**
 * @brief Chain Environment of Symbol Tables (stack)
 *
 * @details
 */
class SymbolTableEnv {
public:
    using SymbolTable = std::unordered_map<std::string, Persona>;

    SymbolTableEnv() : _chain_env() { _chain_env.emplace_back(); }

    /*SymbolTableEnv(const SymbolTableEnv& other) : _chain_env(other._chain_env)
    {
    }*/

    /**
     * @brief Checks if there is Persona with this id in the Env
     *
     * @return bool
     */
    bool exists(const std::string&) noexcept;

    /**
     * @brief Add persona to Env
     *
     * @details Creates new link in Env chain if Persona with the same id
     * already exists
     * @return Reference of created persona
     */
    Persona& add_persona(const std::string& persona_id) noexcept;

    /**
     * @brief Add member/change in persona
     *
     * @details Creates new link in Env chain if member with same id in
     * Persona already exists
     * @return bool `false` if there is no such Persona in Env
     */
    /// TODO: optimize usage: we don't need to create new link to add totally
    /// new variable
    bool add_info_to(const std::string& persona_id,
            const std::string&          member_id,
            const std::string&          member_parameter) noexcept;

    /**
     * @throw runtime_error if there is no such Persona in Env
     */
    [[nodiscard]] Persona& get_persona(const std::string& persona_id);

private:
    std::deque<SymbolTable> _chain_env;
};

}  // namespace vino
