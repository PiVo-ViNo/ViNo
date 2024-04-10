#pragma once

#include "custom_errors.hpp"
#include <glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>

///TODO: Write converter between u32string and string, also char32_t[]
namespace vino {

struct Character;
template <typename _Ch>
class FreeTypeLib;
template <typename _Ch>
class FreeTypeFace;
template <typename _Ch>
class Font;
template <typename _Ch>
class FontsCollection;

struct Character {
    unsigned int texture_id;  // ID handle of the glyph texture
    glm::ivec2   size;        // Size of glyph
    glm::ivec2   bearing;     // Offset from baseline to left/top of glyph
    int advance;     // Offset to advance to next glyph
};

/**
 @brief Wrapper around FT_Library, not for right ahead usage
*/
template <typename _Ch>
class FreeTypeLib {
public:
    friend class FontsCollection<_Ch>;

    FreeTypeLib()
    {
        // return Error code, so 0 is success (well, i didn't this C code)
        if (FT_Init_FreeType(&_native_ft_lib)) {
            throw WindowError(
                    "ERROR::FREETYPE::Couldn't init FreeType2 library");
        }
    }

    ~FreeTypeLib() { FT_Done_FreeType(_native_ft_lib); }

    FreeTypeLib(FreeTypeLib&&);
    FreeTypeLib& operator=(FreeTypeLib&& other);

    FreeTypeLib(const FreeTypeLib&) = delete;
    FreeTypeLib& operator=(const FreeTypeLib&) = delete;

private:
    FT_Library _native_ft_lib{};
};

/**
 @brief Wrapper around FT_Face, not for right ahead usage
*/
template <typename _Ch>
class FreeTypeFace {
public:
    using char_type = _Ch;

    Character& get_char(const char_type& ch);

    FreeTypeFace(FreeTypeFace&& other);

    FreeTypeFace& operator=(const FreeTypeFace&) = delete;

    ~FreeTypeFace() { FT_Done_Face(_native_ft_face); }

private:
    friend class FontsCollection<char_type>;

    FreeTypeFace(
            FT_Library& ft_lib, std::string font_path, int pxl_size);

    /// @param `pixel_height` can be omitted, makes it equal to `pixel_width`
    void set_pixel_size(
            int pixel_width, int pixel_height = 0);

    Character& load_symbol(char_type ch, bool in_cycle = false);
    void       load_ascii();

    std::string                    _font_path{};
    FT_Face                        _native_ft_face{};
    std::map<char_type, Character> _chars_map{};
};

/**
 * @brief External safe interface for font operations
 * @details This class is safer to operate than FreeTypeFace
 * @tparam _Ch 
 */
template <typename _Ch>
class Font {
public:
    using char_type = _Ch;

    explicit Font(FreeTypeFace<char_type>& face) : _face(face) {}

    void render_str(const std::basic_string<char_type>& str, unsigned int vbo,
            glm::ivec2 ll_pos, float scale) const;

    /// @return how many chars from str was rendered
    std::size_t render_str_inbound(const std::basic_string<char_type>& str,
            unsigned int vbo, glm::ivec2 ll_pos, float scale,
            int x_bound) const;

    [[nodiscard]] glm::ivec2 get_dimensions_of(
            const std::string& str, float scale) const;

private:
    FreeTypeFace<char_type>& _face;
};

/**
 * @brief Main class for fonts configurations
 * @details All configuration related actions with fonts must go through this
 * class: adding, changing size, etc. Inside this class all operations go
 * through std::map of fonts
 * @tparam _Ch char_type
 */
template <typename _Ch>
class FontsCollection {
public:
    using char_type = _Ch;

    FontsCollection<char_type>() = default;
    // explicit FontCollection(FreeTypeLib& ft_lib) : _ft_lib(ft_lib) {}

    bool add_font(const std::string& font_path, int size);
    bool add_font_with_ascii(const std::string& font_path, int size);

    Font<char_type> operator[](const std::string& font_name);

private:
    FreeTypeLib<char_type> _ft_lib{};
    /// Map of `font name` -> FreeTypeFace
    std::map<std::string, FreeTypeFace<char_type>> _faces{};
};

}  // namespace vino