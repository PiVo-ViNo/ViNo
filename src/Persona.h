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

#include <string>
#include <unordered_map>

namespace vino {

class Persona {
public:

    Persona(const std::string& name, const std::string& path) :
        _name(name), _path(path)
    {
    }

    Persona() : Persona("", "") {}

    Persona(const Persona& other) :
        _name(other._name),
        _path(other._path),
        _fgs_title_path(other._fgs_title_path)
    {
    }

    Persona(Persona&& other) noexcept :
        _name(std::move(other._name)),
        _path(std::move(other._path)),
        _fgs_title_path(std::move(other._fgs_title_path))
    {
    }

    ~Persona() = default;

    /// Fast comparison, only by `_name` field
    constexpr bool operator==(const Persona&); 

    inline std::string get_name() const noexcept;

    /// Add foreground variable, member of persona
    void add_fg_var(const std::string&, const std::string&);
    void add_fg_var(std::string&&, std::string&&);

    /// Remove foreground variable, not really intended to use
    void rm_fg_var(const std::string&);

private:
    const std::string _name;
    const std::string _path;
    /// Hashmap with `[key=title, value=path/to/img]` foregrounds (members)
    std::unordered_map<const std::string, const std::string> _fgs_title_path;
};

}  // namespace vino

namespace std {

template <>
struct hash<vino::Persona> {
    std::size_t operator()(const vino::Persona& persona) const noexcept
    {
        return std::hash<std::string>{}(persona.get_name());
    }
};

} // namespace std