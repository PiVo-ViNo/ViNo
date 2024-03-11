/*
 * SymbolTable.cpp
 *
 * Created on Sun Mar 10 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */

#include "SymbolTable.h"
#include <stdexcept>

namespace vino {

bool SymbolTableEnv::exists(const std::string& persona_id) noexcept
{
    if (_chain_env.front().find(persona_id) != _chain_env.front().end()) {
        return true;
    }
    return false;
}

Persona& SymbolTableEnv::add_persona(const std::string& persona_id) noexcept
{
    auto persona = _chain_env.front().find(persona_id);
    if (persona != _chain_env.front().end()) {
        return _chain_env.front().emplace(persona_id).first->second;
    }
    _chain_env.emplace_front(_chain_env.front());
    _chain_env.front().extract(persona_id);
    return _chain_env.front().emplace(persona_id).first->second;
}

bool SymbolTableEnv::add_info_to(const std::string& persona_id,
                                 const std::string& member_id,
                                 const std::string& member_parameter) noexcept
{
    auto persona_it = _chain_env.front().find(persona_id);
    if (persona_it == _chain_env.front().end()) {
        return false;
    }
    if (!persona_it->second.add_fg_var(member_id, member_parameter)) {
        _chain_env.emplace_front(_chain_env.front());
        Persona persona = _chain_env.front().find(persona_id)->second;
        persona.rm_fg_var(member_id);
        persona.add_fg_var(member_id, member_parameter);
    }
    return true;
}

Persona& SymbolTableEnv::get_persona(const std::string& persona_id)
{
    auto persona_it = _chain_env.front().find(persona_id);
    if (persona_it == _chain_env.front().end()) {
        throw std::runtime_error("ENV:Error: no persona " + persona_id);
    }
    return persona_it->second;
}

}  // namespace vino