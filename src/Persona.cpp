#include <string>

#include "Persona.h"

namespace vino {

std::string Persona::get_name() const noexcept
{
    return _name;
}

std::string Persona::get_path() const noexcept
{
    return _path;
}

std::string Persona::get_main_fg() const noexcept
{
    return _main_fg;
}

bool Persona::set_path(const std::string& path) noexcept
{
    if (!_path.empty()) {
        return false;
    }
    _path = path;
    return true;
}

bool Persona::set_name(const std::string& name) noexcept
{
    if (_name.empty()) {
        _name = name;
        return true;
    }
    return false;
}

bool Persona::set_main_fg(const std::string& main_fg_path) noexcept
{
    if (_main_fg.empty()) {
        _main_fg = main_fg_path;
        return true;
    }
    return false;
}

bool Persona::add_fg_var(const std::string& member_name,
                                const std::string& path_value)
{
    return _fgs_member_path.emplace(member_name, path_value).second;
}

bool Persona::add_fg_var(std::string&& member_name,
                                std::string&& path_value)
{
    return _fgs_member_path.emplace(member_name, path_value).second;
}

void Persona::rm_fg_var(const std::string& member_name)
{
    _fgs_member_path.erase(member_name);
}

std::string Persona::get_fg(const std::string& id) const noexcept
{
    auto fg_it = _fgs_member_path.find(id);
    if (fg_it == _fgs_member_path.end()) {
        return "";
    }
    return fg_it->second;
}
}  // namespace vino