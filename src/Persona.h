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

#pragma once

#include <string>
#include <unordered_map>

namespace vino {

class Persona {
public:
    Persona(const std::string& name, const std::string& path) :
        _name(name), _path(path)
    {
    }

    explicit Persona(const std::string& name) : Persona(name, "") {}

    Persona() : Persona("", "") {}

    Persona(const Persona& other) :
        _name(other._name),
        _path(other._path),
        _fgs_member_path(other._fgs_member_path)
    {
    }

    Persona(Persona&& other) noexcept :
        _name(std::move(other._name)),
        _path(std::move(other._path)),
        _fgs_member_path(std::move(other._fgs_member_path))
    {
    }

    ~Persona() = default;

    std::string get_name() const noexcept;
    std::string get_path() const noexcept;

    /// Set path in case it wasn't set in construction
    /**
    @details `_path` can be changed only if it wasn't set in construction,
    otherwise this method won't do anything!
    @return `true` if `_path` was changed, otherwise `false`
    */
    bool set_path(const std::string&) noexcept;

    /// Set name (which displayed on screen, not Id)
    /// @warning IT'S NOT ID OF PERSONA AS VARIABLE
    bool set_name(const std::string&) noexcept;

    /// Set main foreground (displayed by default)
    /// @return `true` if it was empty and was changed, `false` otherwise
    bool set_main_fg(const std::string&) noexcept;

    /// Add foreground variable, member of persona
    /// @details Don't change variable if there is already one,
    /// must be used in pair with Env chain to create new instance of Persona
    /// with new variable value
    bool add_fg_var(const std::string& member_name,
                           const std::string& path_value);
    bool add_fg_var(std::string&& member_name, std::string&& path_value);

    /// Remove foreground variable, not really intended to use
    void rm_fg_var(const std::string&);

    /// @return empty string, if there is no member with such id
    std::string get_fg(const std::string& id) const noexcept;

private:
    std::string _name;  /// NOT ID, but displayed name
    std::string _path;
    std::string _main_fg;
    /// Hashmap with `[key=title, value=path/to/img]` foregrounds (members)
    std::unordered_map<const std::string, const std::string> _fgs_member_path{};
};

}  // namespace vino