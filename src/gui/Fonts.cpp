#include "Fonts.hpp"

#include <freetype/fttypes.h>
#include <glad/glad.h>
// #include "freetype/freetype.h"
#include <filesystem>
#include <array>

namespace vino {

// FreeTypeLib ----------------------------------------------------------------
// ----------------------------------------------------------------------------

template <typename _Ch>
FreeTypeLib<_Ch>::FreeTypeLib(FreeTypeLib&& other)
{
    _native_ft_lib = other._native_ft_lib;
    other._native_ft_lib = nullptr;
}

template <typename _Ch>
FreeTypeLib<_Ch>& FreeTypeLib<_Ch>::operator=(FreeTypeLib<_Ch>&& other)
{
    FT_Library ptrTemp = other._native_ft_lib;
    other._native_ft_lib = this->_native_ft_lib;
    _native_ft_lib = ptrTemp;
    return *this;
}

// FreeTypeFace ---------------------------------------------------------------
// ----------------------------------------------------------------------------

template <typename _Ch>
FreeTypeFace<_Ch>::FreeTypeFace(FreeTypeFace&& other) :
    _font_path(std::move(other._font_path)),
    _chars_map(std::move(other._chars_map))
{
    FT_Face ptrTemp = other._native_ft_face;
    other._native_ft_face = nullptr;
    _native_ft_face = ptrTemp;
}

template <typename _Ch>
FreeTypeFace<_Ch>::FreeTypeFace(
        FT_Library& ft_lib, std::string font_path, int pxl_size) :
    _font_path(std::move(font_path))
{
    assert(pxl_size > 0);
    if (_font_path.empty()) {
        throw WindowError("ERROR::FREETYPE::Empty path to font");
    }
    if (FT_Error err = FT_New_Face(
                ft_lib, _font_path.c_str(), 0, &_native_ft_face))
    {
        throw WindowError("ERROR::FREETYPE " + std::to_string(err)
                          + "::Couldn't init FreeTypeFace");
    }
    if (FT_Error err = FT_Set_Pixel_Sizes(
                _native_ft_face, 0, static_cast<FT_UInt>(pxl_size)))
    {
        FT_Done_Face(_native_ft_face);
        throw WindowError("ERROR::FREETYPE " + std::to_string(err)
                          + "::Couldn't set pixel size");
    }
}

template <typename _Ch>
void FreeTypeFace<_Ch>::set_pixel_size(int pixel_width, int pixel_height)
{
    assert(pixel_width > 0 && pixel_height > 0);
    if (FT_Error err = FT_Set_Pixel_Sizes(_native_ft_face,
                static_cast<FT_UInt>(pixel_width),
                static_cast<FT_UInt>(pixel_height)))
    {
        throw WindowError("ERROR::FREETYPE " + std::to_string(err)
                          + "::Couldn't set pixel size");
    }
}

template <typename _Ch>
Character& FreeTypeFace<_Ch>::load_symbol(_Ch ch, bool in_cycle)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // Load character glyph in render mode
    if (FT_Error err = FT_Load_Char(
                _native_ft_face, static_cast<FT_ULong>(ch), FT_LOAD_RENDER))
    {
        throw WindowError("ERROR::FREETYPE " + std::to_string(err)
                          + "::Failed to load Glyph");
    }
    // std::cout << ch << " ";
    // generate texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _native_ft_face->glyph->bitmap.width,
            _native_ft_face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
            _native_ft_face->glyph->bitmap.buffer);
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // now store character for later use
    Character character = {texture,
            glm::ivec2(_native_ft_face->glyph->bitmap.width,
                    _native_ft_face->glyph->bitmap.rows),
            glm::ivec2(_native_ft_face->glyph->bitmap_left,
                    _native_ft_face->glyph->bitmap_top),
            static_cast<int>(_native_ft_face->glyph->advance.x)};
    auto it = _chars_map.insert(std::pair<_Ch, Character>(ch, character)).first;
    if (!in_cycle) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    return it->second;
}

template <typename _Ch>
void FreeTypeFace<_Ch>::load_ascii()
{
    for (_Ch ch = 0; ch < 127; ch++) {
        load_symbol(ch, true);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

template <typename _Ch>
Character& FreeTypeFace<_Ch>::get_char(const _Ch& ch)
{
    auto it = _chars_map.find(ch);
    if (it == _chars_map.end()) {
        return load_symbol(ch);
    }
    return it->second;
}

// Font -----------------------------------------------------------------------
// ----------------------------------------------------------------------------

template <typename _Ch>
void Font<_Ch>::render_str(const std::basic_string<_Ch>& str, unsigned int vbo,
        glm::ivec2 lowleft_pos, float scale) const
{
    if (str.empty()) {
        return;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glm::vec2 ll_pos(std::move(lowleft_pos));
    for (const _Ch& c : str) {
        Character ch = _face.get_char(c);

        float xpos = ll_pos.x + static_cast<float>(ch.bearing.x) * scale;
        float ypos =
                ll_pos.y - static_cast<float>(ch.size.y - ch.bearing.y) * scale;

        float w = static_cast<float>(ch.size.x) * scale;
        float h = static_cast<float>(ch.size.y) * scale;

        std::array<std::array<float, 4>, 6> vertices = {
                {{xpos, ypos + h, 0.0f, 0.0f}, {xpos, ypos, 0.0f, 1.0f},
                 {xpos + w, ypos, 1.0f, 1.0f},

                 {xpos, ypos + h, 0.0f, 0.0f},
                 {xpos + w, ypos, 1.0f, 1.0f},
                 {xpos + w, ypos + h, 1.0f, 0.0f}}
        };

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.texture_id);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0,
                sizeof(float) * vertices.size() * vertices.data()->size(),
                vertices.data());

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        ll_pos.x += static_cast<float>(ch.advance >> 6) * scale;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

template <typename _Ch>
std::size_t Font<_Ch>::render_str_inbound(
        const std::basic_string<char_type>& str, unsigned int vbo,
        glm::ivec2 lowleft_pos, float scale, int x_bound) const
{
    if (str.empty()) {
        return 0;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    std::size_t count_chars = 0;

    glm::vec2 ll_pos(std::move(lowleft_pos));
    for (const _Ch& c : str) {
        Character ch = _face.get_char(c);

        float xpos = ll_pos.x + static_cast<float>(ch.bearing.x) * scale;
        float ypos =
                ll_pos.y - static_cast<float>(ch.size.y - ch.bearing.y) * scale;
        if (c == '\n'
                || ll_pos.x + static_cast<float>(ch.advance >> 6) * scale
                           >= static_cast<float>(x_bound))
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            return ++count_chars;
        }

        float w = static_cast<float>(ch.size.x) * scale;
        float h = static_cast<float>(ch.size.y) * scale;

        std::array<std::array<float, 4>, 6> vertices = {
                {{xpos, ypos + h, 0.0f, 0.0f}, {xpos, ypos, 0.0f, 1.0f},
                 {xpos + w, ypos, 1.0f, 1.0f},

                 {xpos, ypos + h, 0.0f, 0.0f},
                 {xpos + w, ypos, 1.0f, 1.0f},
                 {xpos + w, ypos + h, 1.0f, 0.0f}}
        };

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.texture_id);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0,
                sizeof(float) * vertices.size() * vertices.data()->size(),
                vertices.data());

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        ll_pos.x += static_cast<float>(ch.advance >> 6) * scale;
        count_chars++;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    return count_chars;
}

template <typename _Ch>
glm::ivec2 Font<_Ch>::get_dimensions_of(
        const std::string& str, float scale) const
{
    glm::ivec2 dimensions{};

    for (const char& c : str) {
        Character ch = _face.get_char(static_cast<_Ch>(c));

        dimensions.x +=
                static_cast<int>(static_cast<float>(ch.advance >> 6) * scale);
        if (ch.bearing.y > dimensions.y) {
            dimensions.y = ch.bearing.y;
        }
    }

    return dimensions;
}

// FontCollection -------------------------------------------------------------
// ----------------------------------------------------------------------------

template <typename _Ch>
bool FontsCollection<_Ch>::add_font_with_ascii(
        const std::string& font_path, int size)
{
    assert(size > 0);
    std::filesystem::path temp_path = std::filesystem::path(font_path);
    if (!std::filesystem::exists(temp_path) || temp_path.extension() != ".ttf")
    {
        throw WindowError("No file \"" + font_path
                          + "\" or it doesn't have .ttf extension");
    }
    std::string font_name = temp_path.stem().string();

    auto pair_it = _faces.emplace(font_name,
            FreeTypeFace<_Ch>(_ft_lib._native_ft_lib, font_path, size));
    if (!pair_it.second) {
        return false;
    }
    pair_it.first->second.load_ascii();
    return true;
}

template <typename _Ch>
Font<_Ch> FontsCollection<_Ch>::operator[](const std::string& font_name)
{
    auto it = _faces.find(font_name);
    if (it == _faces.end()) {
        throw WindowError("No font with name \"" + font_name + "\" found");
    }
    return Font<_Ch>(it->second);
}

// explicit instantiations
template class FreeTypeLib<char>;
template class FreeTypeLib<char16_t>;
template class FreeTypeLib<char32_t>;

template class FreeTypeFace<char>;
template class FreeTypeFace<char16_t>;
template class FreeTypeFace<char32_t>;

template class Font<char>;
template class Font<char16_t>;
template class Font<char32_t>;

template class FontsCollection<char>;
template class FontsCollection<char16_t>;
template class FontsCollection<char32_t>;

}  // namespace vino