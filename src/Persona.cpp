#include "Persona.h"

#include <string>


namespace vino {

constexpr bool Persona::operator==(const Persona& rhs)
{
    return (this->_name == rhs._name);
}

inline std::string Persona::get_name() const noexcept { return _name; }

void Persona::add_fg_var(const std::string& title_key,
                         const std::string& path_value)
{
    _fgs_title_path.emplace(title_key, path_value);
}

void Persona::add_fg_var(std::string&& title_key, std::string&& path_value) 
{
    _fgs_title_path.emplace(title_key, path_value);
}

void Persona::rm_fg_var(const std::string& title_key) {
    _fgs_title_path.erase(title_key);
}

}  // namespace vino