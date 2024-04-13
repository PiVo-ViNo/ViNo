#include "Box.hpp"
#include <string>
#include <string_view>
#include <stuff.hpp>

namespace vino {

// IBox -----------------------------------------------------------------------
// ----------------------------------------------------------------------------

IBox::IBox(const glm::ivec2& low_left_pos, int width, int height,
        Window& parent_window) :
    _win(parent_window), _ll_pos(low_left_pos), _width(width), _height(height)
{
    // std::cout << "Created Box: " << _width << "x" << _height << std::endl;
}

bool IBox::is_cursor_in() const
{
    std::pair<int, int> pair = _win.get_cursor_pos();
    if (pair.first < 0 || pair.second < 0) {
        return false;
    }
    return (pair.first >= _ll_pos.x && pair.second >= _ll_pos.y
            && pair.first <= _ll_pos.x + _width
            && pair.second <= _ll_pos.y + _height);
}

bool IBox::is_clicked() const
{
    return is_cursor_in() && _win.is_clicked();
}

glm::ivec2 IBox::get_low_left_pos() const
{
    return _ll_pos;
}

int IBox::get_width() const
{
    return _width;
}

int IBox::get_height() const
{
    return _height;
}

Window& IBox::get_window() const
{
    return _win;
}

// ITextureColorBox -----------------------------------------------------------
// ----------------------------------------------------------------------------

ITextureColorBox::ITextureColorBox(const glm::ivec2& low_left_pos, int width,
        int height, Window& parent_window, const ImgData& img,
        const glm::vec4& color, int GL_TYPE_DRAW) :
    IBox(low_left_pos, width, height, parent_window),
    _box_shader("./shaders/basicVertex.glsl", "shaders/basicFrag.glsl"),
    _color(color)
{
    if (GL_TYPE_DRAW != GL_STATIC_DRAW && GL_TYPE_DRAW != GL_DYNAMIC_DRAW) {
        throw WindowError(
                "Wrong GL_<TYPE>_DRAW in constructor ITextureColorBox");
    }
    glGenBuffers(1, &_box_vertex_buffer);
    glGenVertexArrays(1, &_box_vertex_array);
    glGenBuffers(1, &_box_element_buffer);
    _box_texture = configure_texture(img, 0);
    _box_shader.use();
    _box_shader.setInt("uTexture", 0);

    std::array<std::array<int, 5>, 4> corners = {
            {{_ll_pos.x, _ll_pos.y, 0, 0, 0},
             {_ll_pos.x, _ll_pos.y + _height, 0, 0, 1},
             {_ll_pos.x + _width, _ll_pos.y + _height, 0, 1, 1},
             {_ll_pos.x + _width, _ll_pos.y, 0, 1, 0}}
    };
    std::array<int, 6> elem_indices = {0, 1, 2, 2, 3, 0};

    // init buffers for box
    glBindVertexArray(_box_vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, _box_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER,
            sizeof(int) * corners.size() * corners.data()->size(),
            corners.data(), GL_TYPE_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _box_element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * elem_indices.size(),
            elem_indices.data(), GL_TYPE_DRAW);

    glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, 5 * sizeof(int), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_INT, GL_FALSE, 5 * sizeof(int),
            reinterpret_cast<void*>(3 * sizeof(int)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ITextureColorBox::render() const
{
    ITextureColorBox::render(-1.0f);
}

void ITextureColorBox::render(float uniform_alpha) const
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _box_texture);

    _box_shader.use();
    /// TODO:Q: Usage of static_cast of float-matrix at all, can be int?
    glm::mat4 projection =
            glm::ortho(0.0f, static_cast<float>(_win.get_width()), 0.0f,
                    static_cast<float>(_win.get_height()));
    _box_shader.setMat4FloatV("uProjection", projection);
    _box_shader.setVec4Float("uColor", _color.x, _color.y, _color.z, _color.w);
    _box_shader.setFloat("uAlpha", uniform_alpha);

    glBindVertexArray(_box_vertex_array);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ITextureColorBox::change_texture(const ImgData& new_img)
{
    glDeleteTextures(1, &_box_texture);
    _box_texture = configure_texture(new_img, 0);
}

void ITextureColorBox::change_color(const glm::vec4& new_color)
{
    _color = new_color;
}

glm::vec4 ITextureColorBox::get_color() const
{
    return _color;
}

ITextureColorBox::~ITextureColorBox()
{
    glDeleteVertexArrays(1, &_box_vertex_array);
    glDeleteBuffers(1, &_box_vertex_buffer);
    glDeleteBuffers(1, &_box_element_buffer);
    glDeleteTextures(1, &_box_texture);
}

// IStaticBox -----------------------------------------------------------------
// ----------------------------------------------------------------------------

IStaticBox::IStaticBox(const glm::ivec2& low_left_pos, int width, int height,
        Window& parent_window, const glm::vec4& color, const ImgData& img) :
    ITextureColorBox(low_left_pos, width, height, parent_window, img, color,
            GL_STATIC_DRAW)
{
    // std::cout << "IStaticBox created" << std::endl;
}

IStaticBox::IStaticBox(const glm::ivec2& low_left_pos, int width, int height,
        Window& parent_window, const ImgData& img, const glm::vec4& color) :
    ITextureColorBox(low_left_pos, width, height, parent_window, img, color,
            GL_STATIC_DRAW)
{
    // std::cout << "IStaticBox created" << std::endl;
}

// IDynamicBox ----------------------------------------------------------------
// ----------------------------------------------------------------------------

void IDynamicBox::recalculate_corners()
{
    _corners = {
            {{_ll_pos.x, _ll_pos.y, 0, 0, 0},
             {_ll_pos.x, _ll_pos.y + _height, 0, 0, 1},
             {_ll_pos.x + _width, _ll_pos.y + _height, 0, 1, 1},
             {_ll_pos.x + _width, _ll_pos.y, 0, 1, 0}}
    };

    glBindVertexArray(_box_vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, _box_vertex_buffer);

    glBufferSubData(GL_ARRAY_BUFFER, 0,
            sizeof(int) * _corners.size() * _corners.data()->size(),
            _corners.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

glm::ivec2 IDynamicBox::move_no_clip(const glm::ivec2& direction)
{
    _ll_pos += direction;
    recalculate_corners();
    return _ll_pos;
}

glm::ivec2 IDynamicBox::move_with_clip(const glm::ivec2& direction)
{
    glm::ivec2 result_vec = _ll_pos + direction;
    if (result_vec.x < 0) {
        result_vec.x = 0;
    } else if (result_vec.x + _width > _win.get_width()) {
        result_vec.x = _win.get_width() - _width;
    }
    if (result_vec.y < 0) {
        result_vec.y = 0;
    } else if (result_vec.y + _height > _win.get_height()) {
        result_vec.y = _win.get_height() - _height;
    }
    _ll_pos = result_vec;
    recalculate_corners();
    return result_vec;
}

glm::ivec2 IDynamicBox::resize_no_clip(const glm::ivec2& new_dimension)
{
    assert(new_dimension.x >= 0 && new_dimension.y >= 0);
    _width = new_dimension.x;
    _height = new_dimension.y;

    recalculate_corners();
    return new_dimension;
}

glm::ivec2 IDynamicBox::resize_with_clip(const glm::ivec2& new_dimension)
{
    if (_ll_pos.x + new_dimension.x > _win.get_width()) {
        _width = _win.get_width() - _ll_pos.x;
    } else {
        _width = new_dimension.x;
    }
    if (_ll_pos.y + new_dimension.y > _win.get_height()) {
        _height = _win.get_height() - _ll_pos.y;
    } else {
        _height = new_dimension.y;
    }
    recalculate_corners();
    return {_width, _height};
}

IDynamicBox::IDynamicBox(const glm::ivec2& low_left_pos, int width, int height,
        Window& parent_window, const glm::vec4& color, const ImgData& img) :
    ITextureColorBox(low_left_pos, width, height, parent_window, img, color,
            GL_DYNAMIC_DRAW),
    _corners({
            {{_ll_pos.x, _ll_pos.y, 0, 0, 0},
             {_ll_pos.x, _ll_pos.y + _height, 0, 0, 1},
             {_ll_pos.x + _width, _ll_pos.y + _height, 0, 1, 1},
             {_ll_pos.x + _width, _ll_pos.y, 0, 1, 0}}
})
{
}

IDynamicBox::IDynamicBox(const glm::ivec2& low_left_pos, int width, int height,
        Window& parent_window, const ImgData& img, const glm::vec4& color) :
    ITextureColorBox(low_left_pos, width, height, parent_window, img, color,
            GL_DYNAMIC_DRAW),
    _corners({
            {{_ll_pos.x, _ll_pos.y, 0, 0, 0},
             {_ll_pos.x, _ll_pos.y + _height, 0, 0, 1},
             {_ll_pos.x + _width, _ll_pos.y + _height, 0, 1, 1},
             {_ll_pos.x + _width, _ll_pos.y, 0, 1, 0}}
})
{
}

// Text -----------------------------------------------------------------------
// ----------------------------------------------------------------------------

template <typename _Ch>
void TextRenderer<_Ch>::render_text(const std::basic_string<_Ch>& str,
        const Font<_Ch>& font, const glm::vec3& color, const glm::ivec2& ll_pos,
        const Window& window) const
{
    if (str.empty()) {
        return;
    }
    _text_shader.use();
    glm::mat4 projection =
            glm::ortho(0.0f, static_cast<float>(window.get_width()), 0.0f,
                    static_cast<float>(window.get_height()));
    _text_shader.setMat4FloatV("uProjection", projection);
    _text_shader.setVec3Float("uTextColor", color.x, color.y, color.z);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(_text_vertex_array);

    font.render_str(str, _text_vertex_buffer, ll_pos, 1.0);

    glBindVertexArray(0);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
}

template <typename _Ch>
std::size_t TextRenderer<_Ch>::render_text_inbound(
        const std::basic_string_view<_Ch>& str, const Font<_Ch>& font,
        const glm::vec3& color, const glm::ivec2& ll_pos, int x_bound,
        const Window& window) const
{
    if (str.empty()) {
        return 0;
    }
    _text_shader.use();
    glm::mat4 projection =
            glm::ortho(0.0f, static_cast<float>(window.get_width()), 0.0f,
                    static_cast<float>(window.get_height()));
    _text_shader.setMat4FloatV("uProjection", projection);
    _text_shader.setVec3Float("uTextColor", color.x, color.y, color.z);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(_text_vertex_array);

    std::size_t count_chars = font.render_str_inbound(
            str, _text_vertex_buffer, ll_pos, 1.0, x_bound);

    glBindVertexArray(0);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);

    return count_chars;
}

template <typename _Ch>
TextRenderer<_Ch>::TextRenderer() :
    _text_shader("shaders/charVertex.glsl", "shaders/charFrag.glsl")
{
    glGenBuffers(1, &_text_vertex_buffer);
    glGenVertexArrays(1, &_text_vertex_array);

    glBindVertexArray(_text_vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, _text_vertex_buffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

template <typename _Ch>
TextRenderer<_Ch>::~TextRenderer()
{
    glDeleteBuffers(1, &_text_vertex_buffer);
    glDeleteVertexArrays(1, &_text_vertex_array);
}

// StaticTextBox --------------------------------------------------------------
// ----------------------------------------------------------------------------

template <typename _Ch>
StaticTextBox<_Ch>::StaticTextBox(const glm::ivec2& low_left_pos, int width,
        int height, Window& parent_window, const glm::vec4& color,
        const ImgData& img) :
    IStaticBox(low_left_pos, width, height, parent_window, img, color)
{
    _text = std::make_unique<TextRenderer<char_type>>();
}

template <typename _Ch>
StaticTextBox<_Ch>::StaticTextBox(const glm::ivec2& low_left_pos, int width,
        int height, Window& parent_window, const ImgData& img,
        const glm::vec4& color) :
    IStaticBox(low_left_pos, width, height, parent_window, img, color)
{
    _text = std::make_unique<TextRenderer<char_type>>();
}

template <typename _Ch>
std::size_t StaticTextBox<_Ch>::render_text(const std::basic_string<_Ch>& text,
        const Font<_Ch>& font, const glm::vec4& color) const
{
    std::basic_string<char_type> white_spaces =
            convert_str<char, char_type>(" \n");
    const int glyph_max_height = font.size();

    int y_cur = _ll_pos.y
                + std::max(_height - glyph_max_height * 7 / 5,
                        (_height - glyph_max_height) / 2);
    int x_cur = _ll_pos.x + font.size();

    // Get the words from str, but if str is longer than whole box, sent it as
    // it is to the renderer, tearing it apart in some places there
    std::basic_string_view<char_type> text_view(text);
    std::basic_string_view<char_type> text_view_word =
            text_view.substr(0, text_view.find_first_of(white_spaces));

    std::size_t rendered_chars = _text->render_text_inbound(text_view_word,
            font, color, {_ll_pos.x + font.size(), y_cur},
            _ll_pos.x + _width - 10, _win);
    x_cur += font.get_dimensions_of(text_view_word, 1.0).x;

    while (rendered_chars < text.size()) {
        text_view_word = text_view.substr(rendered_chars,
                text_view.find_first_of(white_spaces, rendered_chars + 1)
                        - rendered_chars);
        int word_length = font.get_dimensions_of(text_view_word, 1.0).x;

        if (text_view_word.starts_with('\n')
                || (x_cur + word_length > _ll_pos.x + _width - font.size()
                        && word_length < _width))
        {
            y_cur -= glyph_max_height * 7 / 5;
            if (y_cur < _ll_pos.y + glyph_max_height / 4) {
                break;
            }
            x_cur = _ll_pos.x + font.size();
            std::size_t skipped =
                    text_view_word.find_first_not_of(white_spaces);
            rendered_chars += skipped;
            text_view_word.remove_prefix(skipped);
            word_length = font.get_dimensions_of(text_view_word, 1.0).x;
        }

        /// TODO:Q: don't render with *_inbound()?
        rendered_chars += _text->render_text_inbound(text_view_word, font,
                color, {x_cur, y_cur}, _ll_pos.x + _width - font.size(), _win);
        x_cur += word_length;
    }
    return rendered_chars;
}

// ForegroundFigure -----------------------------------------------------------
// ----------------------------------------------------------------------------

ForegroundFigure::ForegroundFigure(const glm::ivec2& low_left_pos, int width,
        int height, Window& parent_window, const ImgData& img,
        const glm::vec4& color) :
    IDynamicBox(low_left_pos, width, height, parent_window, img, color)
{
}

// FullscreenTexture ----------------------------------------------------------
// ----------------------------------------------------------------------------

FullscreenTexture::FullscreenTexture(
        Window& window, const ImgData& img, const glm::vec4& color) :
    IStaticBox(
            {0, 0}, window.get_width(), window.get_height(), window, img, color)
{
}

FullscreenTexture::FullscreenTexture(
        Window& window, const glm::vec4& color, const ImgData& img) :
    IStaticBox(
            {0, 0}, window.get_width(), window.get_height(), window, img, color)
{
}

// Button ---------------------------------------------------------------------
// ----------------------------------------------------------------------------

template <typename _Ch>
Button<_Ch>::Button(const glm::ivec2& low_left_pos, int width, int height,
        Window& parent_window, const glm::vec4& font_color,
        const std::basic_string<char_type>& title, const Font<char_type>& font,
        const ImgData& img, const glm::vec4& box_color) :
    IStaticBox(low_left_pos, width, height, parent_window, img, box_color),
    _title(std::move(title)),
    _font(font),
    _title_color(font_color)
{
    _text = std::make_unique<TextRenderer<char_type>>();
}

template <typename _Ch>
void Button<_Ch>::render() const
{
    IStaticBox::render();
    _text->render_text(_title, _font, _title_color,
            {_ll_pos.x + 10, _ll_pos.y + (_height - _font.size()) / 2}, _win);
}

// LowBox ---------------------------------------------------------------------
// ----------------------------------------------------------------------------

template <typename _Ch>
LowBox<_Ch>::LowBox(Window& parent_window, const glm::ivec2& box_ll_pos,
        const glm::ivec2& box_dimensions, const glm::vec4& box_color,
        const glm::vec4& title_color, const Font<char_type>& font) :
    _text_box(box_ll_pos, box_dimensions.x, box_dimensions.y, parent_window,
            box_color),
    _name_box({box_ll_pos.x + 10, box_dimensions.y + box_ll_pos.y},
            box_dimensions.x / 4, font.size() * 2, parent_window, title_color),
    _font(font)
{
    _box_ll_pos = box_ll_pos;
    _box_dimensions = box_dimensions;
    _title_ll_pos = _name_box.get_low_left_pos();
    _title_dimensions = {_name_box.get_width(), _name_box.get_height()};
    _box_color = _text_box.get_color();
}

template <typename _Ch>
LowBox<_Ch>::LowBox(const LowBox<char_type>& other) :
    LowBox(other._text_box.get_window(), other._box_ll_pos,
            other._box_dimensions, other._box_color, other._title_box_color,
            other._font)
{
}

template <typename _Ch>
void LowBox<_Ch>::render()
{
    _text_box.render();
    _name_box.render();
    _text_pos = _text_box.render_text(_text, _font,
            {1.0f - _box_color.r, 1.0f - _box_color.g, 1.0f - _box_color.b,
                    1.0f});
    _name_box.render_text(_name_text, _font,
            {1.0f - _name_box.get_color().r, 1.0f - _name_box.get_color().g,
                    1.0f - _name_box.get_color().b, 1.0f});
}

template <typename _Ch>
void LowBox<_Ch>::render(
        const std::basic_string<_Ch>& name, const std::basic_string<_Ch>& text)
{
    update_text(text);
    update_name(name);
    render();
}

template <typename _Ch>
void LowBox<_Ch>::update_text(const std::basic_string<char_type>& text)
{
    _text = text;
}

template <typename _Ch>
bool LowBox<_Ch>::next_slide()
{
    if (_text_pos == _text.size()) {
        // almost all implementations don't deallocate when call clear(),
        // so it's not really bad for performance
        _text.clear();
        return false;
    }
    _text.erase(0, _text_pos);
    _text_pos = 0;
    return true;
}

template <typename _Ch>
inline void LowBox<_Ch>::add_text(const std::basic_string<char_type>& text)
{
    _text += text;
}

template <typename _Ch>
void LowBox<_Ch>::update_name(const std::basic_string<char_type>& name)
{
    _name_text = name;
}

template <typename _Ch>
std::basic_string<_Ch> LowBox<_Ch>::get_text() const
{
    return _text;
}

// explicit instantiations
template class TextRenderer<char>;
template class TextRenderer<char16_t>;
template class TextRenderer<char32_t>;

template class StaticTextBox<char>;
template class StaticTextBox<char16_t>;
template class StaticTextBox<char32_t>;

template class Button<char>;
template class Button<char16_t>;
template class Button<char32_t>;

template class LowBox<char>;
template class LowBox<char16_t>;
template class LowBox<char32_t>;

}  // namespace vino