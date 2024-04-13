#pragma once

#include "Fonts.hpp"
#include "ImgData.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include <glm.hpp>

#include <array>
#include <gui/Box.hpp>
#include <memory>
#include <string_view>
#include <string>

namespace vino {

class IBox;
class IMovable;
class IResizable;
class ITextureColorBox;
class IDynamicBox;
class IStaticBox;

template <typename _Ch>
class TextRenderer;

class FullscreenTexture;
template <typename _Ch>
class StaticTextBox;
template <typename _Ch>
class Button;
class ForegroundFigure;
template <typename _Ch>
class LowBox;
class SimpleBox;

/**
 @brief Basic abstract class for Boxes
*/
class IBox {
public:
    virtual ~IBox() = default;

    [[nodiscard]] virtual bool       is_cursor_in() const;
    [[nodiscard]] virtual bool       is_clicked() const;
    [[nodiscard]] virtual glm::ivec2 get_low_left_pos() const;
    [[nodiscard]] virtual int        get_width() const;
    [[nodiscard]] virtual int        get_height() const;
    [[nodiscard]] virtual Window&    get_window() const;

protected:
    IBox(const glm::ivec2& low_left_pos, int width, int height,
            Window& parent_window);

    Window&    _win;
    glm::ivec2 _ll_pos;
    int        _width;
    int        _height;
};

class IMovable {
public:
    virtual ~IMovable() = default;
    virtual glm::ivec2 move_no_clip(const glm::ivec2& direction) = 0;
    virtual glm::ivec2 move_with_clip(const glm::ivec2& direction) = 0;
};

class IResizable {
public:
    virtual ~IResizable() = default;
    virtual glm::ivec2 resize_no_clip(const glm::ivec2& new_dimension) = 0;
    virtual glm::ivec2 resize_with_clip(const glm::ivec2& new_dimension) = 0;
};

class ITextureColorBox : public IBox {
public:
    ~ITextureColorBox();

    /**
     * @brief Render with no final custom settings
     */
    virtual void render() const;

    /**
     * @brief Render with custom final alpha uniform value in shader
     *
     * @param uniform_alpha Alpha parameter of final Box texture + color
     */
    virtual void render(float uniform_alpha) const;

    void change_texture(const ImgData& new_img);

    void change_color(const glm::vec4& new_color);

    [[nodiscard]] glm::vec4 get_color() const;

protected:
    ITextureColorBox(const glm::ivec2& low_left_pos, int width, int height,
            Window& parent_window, const ImgData& img, const glm::vec4& color,
            int GL_TYPE_DRAW);

    Shader       _box_shader;
    glm::vec4    _color;
    unsigned int _box_vertex_buffer{};
    unsigned int _box_vertex_array{};
    unsigned int _box_element_buffer{};
    unsigned int _box_texture{};
};

class IStaticBox : public ITextureColorBox {
public:
    virtual ~IStaticBox() = default;

protected:
    // This way, because both ImgData and Color shouldn't be omitted at once
    IStaticBox(const glm::ivec2& low_left_pos, int width, int height,
            Window& parent_window, const glm::vec4& color,
            const ImgData& img = {});

    IStaticBox(const glm::ivec2& low_left_pos, int width, int height,
            Window& parent_window, const ImgData& img,
            const glm::vec4& color = {1.0, 1.0, 1.0, 1.0});
};

class IDynamicBox : public ITextureColorBox,
                    public IMovable,
                    public IResizable {
public:
    virtual ~IDynamicBox() = default;

    /**
     * @brief Move object with no restrictions
     *
     * @param direction Integer vector of simple move action
     * @return glm::ivec2 The new low left corner position of the object
     */
    glm::ivec2 move_no_clip(const glm::ivec2& direction) override;

    /**
     * @brief Move object within boundaries of the parent window
     *
     * @param direction Integer vector of simple move action
     * @return glm::ivec2 The new low left corner position of the object
     */
    glm::ivec2 move_with_clip(const glm::ivec2& direction) override;

    /**
     * @brief Resize object with no restrictions
     *
     * @param new_dimension Integer vector of new Width and Height
     * @return glm::ivec2 The new dimensions of the object
     */
    glm::ivec2 resize_no_clip(const glm::ivec2& new_dimension) override;

    /**
     * @brief Resize object within boundaries of the parent window
     *
     * @param new_dimension Integer vector of new Width and Height
     * @return glm::ivec2 The new dimensions of the object
     */
    glm::ivec2 resize_with_clip(const glm::ivec2& new_dimension) override;

protected:
    IDynamicBox(const glm::ivec2& low_left_pos, int width, int height,
            Window& parent_window, const glm::vec4& color, const ImgData& img);

    IDynamicBox(const glm::ivec2& low_left_pos, int width, int height,
            Window& parent_window, const ImgData& img,
            const glm::vec4& color = {1.0, 1.0, 1.0, 1.0});

private:
    /**
     * @brief Recalculate positions after moving/resizing
     */
    void recalculate_corners();

    std::array<std::array<int, 5>, 4> _corners;
};

/**
 * @brief Class which contains purely OpenGL part
 *
 * @tparam _Ch char_type
 */
template <typename _Ch>
class TextRenderer {
public:
    using char_type = _Ch;

    TextRenderer();

    ~TextRenderer();

    /**
     * @brief Render string without any restrictions
     *
     * @param str Text to render
     * @param font Font from FontsCollection
     * @param color Color in RGBA
     * @param ll_pos Low-left corner position of first char
     * @param window Window where to render text
     */
    void render_text(const std::basic_string<char_type>& str,
            const Font<char_type>& font, const glm::vec3& color,
            const glm::ivec2& ll_pos, const Window& window) const;

    /**
     * @brief Render string within interval [ll_pos.x, x_bound]
     *
     * @param str Text to render
     * @param font Font from FontsCollection
     * @param color Color in RGBA
     * @param ll_pos Low-left corner position of first char
     * @param x_bound Right bound in X coordinates
     * @param window Window where to render text
     * @return std::size_t How many chars from str were rendered
     */
    std::size_t render_text_inbound(const std::basic_string_view<_Ch>& str,
            const Font<_Ch>& font, const glm::vec3& color,
            const glm::ivec2& ll_pos, int x_bound, const Window& window) const;

private:
    Shader       _text_shader;
    unsigned int _text_vertex_buffer{};
    unsigned int _text_vertex_array{};
};

class FullscreenTexture : public IStaticBox {
public:
    FullscreenTexture(
            Window& window, const glm::vec4& color, const ImgData& img = {});

    FullscreenTexture(Window& window, const ImgData& img,
            const glm::vec4& color = {1.0, 1.0, 1.0, 1.0});

    virtual ~FullscreenTexture() = default;
};

template <typename _Ch>
class StaticTextBox : public IStaticBox {
public:
    using char_type = _Ch;

    StaticTextBox(const glm::ivec2& low_left_pos, int width, int height,
            Window& parent_window, const glm::vec4& color,
            const ImgData& img = {});

    StaticTextBox(const glm::ivec2& low_left_pos, int width, int height,
            Window& parent_window, const ImgData& img = {},
            const glm::vec4& color = {1.0, 1.0, 1.0, 1.0});

    virtual ~StaticTextBox() = default;

    /**
     * @brief Renders text within the bounds of the Box
     *
     * @param text Text to render
     * @param font Font from FontsCollection
     * @param color Color in RGBA float format in interval [0.0, 1.0]
     * @return std::size_t up to what position text was rendered
     */
    std::size_t render_text(const std::basic_string<char_type>& text,
            const Font<char_type>& font, const glm::vec4& color) const;

private:
    std::unique_ptr<TextRenderer<char_type>> _text{};
};

template <typename _Ch>
class Button : public IStaticBox {
public:
    using char_type = _Ch;

    Button(const glm::ivec2& low_left_pos, int width, int height,
            Window& parent_window, const glm::vec4& font_color,
            const std::basic_string<char_type>& title,
            const Font<char_type>& font, const ImgData& img = {},
            const glm::vec4& box_color = {1.0, 1.0, 1.0, 1.0});

    void render() const override;

private:
    std::basic_string<char_type>             _title{};
    Font<char_type>                          _font;
    std::unique_ptr<TextRenderer<char_type>> _text{};
    glm::vec4                                _title_color;
};

class ForegroundFigure : public IDynamicBox {
public:
    /**
     * @brief Construct a new Foreground Figure object
     *
     * @param low_left_pos Low-left corner position of image
     * @param width
     * @param height
     * @param parent_window Window where it will be rendered
     * @param img Texture to use
     * @param color Color in RGBA float format in interval [0.0, 1.0]
     */
    ForegroundFigure(const glm::ivec2& low_left_pos, int width, int height,
            Window& parent_window, const ImgData& img,
            const glm::vec4& color = {1.0, 1.0, 1.0, 1.0});
};

template <typename _Ch>
class LowBox {
public:
    using char_type = _Ch;

    /**
     * @brief Construct a new Low Box object
     *
     * @param parent_window Window where it will be rendered
     * @param box_ll_pos Position of low-left corner of box
     * @param box_dimensions Width, Height
     * @param box_color Color in RGBA float format in interval [0.0, 1.0]
     * @param title_color Color in RGBA float format in interval [0.0, 1.0]
     * @param font Font from FontsCollection
     */
    LowBox(Window& parent_window, const glm::ivec2& box_ll_pos,
            const glm::ivec2& box_dimensions, const glm::vec4& box_color,
            const glm::vec4& title_color, const Font<char_type>& font);

    /**
     * @brief Create LowBox with current global settings
     *
     * @warning Don't use this constructor when no LowBox was created yet
     * @throw `WindowError()` in case the other constructor was never called
     *        before this one
     */
    LowBox(const LowBox<char_type>& other);

    /**
     * @brief Render the text previously saved in LowBox
     *
     * @remark Useful in case of long text which won't fit at one time in LowBox
     */
    void render();

    /**
     * @brief Render LowBox with new text
     *
     * @param name Name inside LowBox title
     * @param text New text to render and keep in LowBox
     */
    void render(const std::basic_string<char_type>& name,
            const std::basic_string<char_type>&     text);
    void update_text(const std::basic_string<char_type>& text);
    void add_text(const std::basic_string<char_type>& text);
    void update_name(const std::basic_string<char_type>& name);

    /**
     * @brief Update text to next slide
     *
     * @return true New slide is not empty
     * @return false New slide is empty
     */
    bool next_slide();

private:
    glm::ivec2 _box_ll_pos{};
    glm::ivec2 _box_dimensions{};
    glm::vec4  _box_color{1.0, 1.0, 1.0, 1.0};
    glm::ivec2 _title_ll_pos{};
    glm::ivec2 _title_dimensions{};
    glm::vec4  _title_box_color{1.0, 1.0, 1.0, 1.0};

    StaticTextBox<char_type>     _text_box;
    StaticTextBox<char_type>     _name_box;
    std::basic_string<char_type> _text{};
    std::size_t                  _text_pos{};
    std::basic_string<char_type> _name_text{};
    Font<char_type>              _font;
};

class SimpleBox : public IStaticBox {
public:
    SimpleBox(const glm::ivec2& low_left_pos, int width, int height,
            Window& parent_window, const glm::vec4& color,
            const ImgData& img = {}) :
        IStaticBox(low_left_pos, width, height, parent_window, color, img)
    {
    }

    SimpleBox(const glm::ivec2& low_left_pos, int width, int height,
            Window& parent_window, const ImgData& img,
            const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f}) :
        IStaticBox(low_left_pos, width, height, parent_window, img, color)
    {
    }

private:
};

}  // namespace vino