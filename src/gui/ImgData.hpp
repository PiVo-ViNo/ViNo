#pragma once

#include <glad/glad.h>
#include <string>

namespace vino {

/// TODO: Add callbacks for stbi_load_image and implement LOADBINARYBG
/// instruction with them
struct ImgData {
    int            width{};
    int            height{};
    int            numColorChannels{};
    unsigned char* data{};

    ImgData() = default;

    /**
     * @brief Construct a new Img Data object
     *
     * @param path_to_img
     * @param flipped=true By standard stbi load image with Y axis starting from
     * up, but OpenGL requires Y to start from bottom
     */
    explicit ImgData(const std::string& path_to_img, bool flipped = true);

    ~ImgData();

    [[nodiscard]] bool empty() const { return data == nullptr; }
};

unsigned int configure_texture(
        const ImgData& img, unsigned int texNum, int texParam = GL_REPEAT);

}  // namespace vino