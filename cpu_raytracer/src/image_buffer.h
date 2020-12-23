#pragma once

#include <vector>
#include <string>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

class ImageBuffer
{
    unsigned int m_textureName;
    unsigned int m_framebufferObject;

    int m_width;
    int m_height;

    std::vector<glm::vec3> m_imageData;

    bool m_modified;
    int m_modifiedLower;
    int m_modifiedUpper;

    void resetModified();

public:
    ImageBuffer();
    ~ImageBuffer();

    auto width() const { return m_width; }
    auto height() const { return m_height; }

    auto init() -> bool; //this seems like a c++ anti pattern
    auto destroy() -> void; //same here

    // set a pixel in this image buffer to a specified colour:
    //  - (0,0) is the bottom-left pixel of the image
    //  - colour is RGB given as floating point numbers in the range [0,1]
    auto setPixel(glm::ivec2 pos, glm::vec3 colour) -> void;

    auto render() -> void;

    auto saveToFile(std::string_view imgFileName) -> bool;
};