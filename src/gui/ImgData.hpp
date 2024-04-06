#pragma once

#include <glad/glad.h>
#include <string>

namespace vino {

struct ImgData {
    int            width{};
    int            height{};
    int            numColorChannels{};
    unsigned char* data{};

    ImgData() = default;

    explicit ImgData(const std::string& path_to_img);

    ~ImgData();

    [[nodiscard]] bool empty() const { return data == nullptr; }
};

unsigned int configureTexture(
        const ImgData& img, unsigned int texNum, int texParam = GL_REPEAT);

}  // namespace vino