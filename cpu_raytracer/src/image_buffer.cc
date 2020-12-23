#include "image_buffer.h"
#include <algorithm>
#include <glad/glad.h>
#include <spdlog/spdlog.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

ImageBuffer::ImageBuffer() 
    : m_textureName{0}
    , m_framebufferObject{0}
    , m_width{0}
    , m_height{0}
    , m_modified{false}
    , m_modifiedLower{0}
    , m_modifiedUpper{0}
{
    init();
}

ImageBuffer::~ImageBuffer() 
{
    destroy();
}

auto ImageBuffer::init() -> bool
{
    // retrieve the current viewport size
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    m_width = viewport[2];
    m_height = viewport[3];

    // allocate image data
    m_imageData.resize(m_width * m_height);
    for (int i = 0, k = 0; i < m_height; ++i)
        for (int j = 0; j < m_width; ++j, ++k)
        {
            int p = (i >> 4) + (j >> 4);
            float c = 0.2f + ((p & 1) ? 0.1f : 0.0f);
            m_imageData[k] = glm::vec3(c);
        }

    // allocate texture object
    if (!m_textureName)
        glGenTextures(1, &m_textureName);
    glBindTexture(GL_TEXTURE_RECTANGLE, m_textureName);
    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB, m_width, m_height, 0, GL_RGB,
                 GL_FLOAT, &m_imageData[0]);
    glBindTexture(GL_TEXTURE_RECTANGLE, 0);
    resetModified();

    // allocate framebuffer object
    if (!m_framebufferObject)
        glGenFramebuffers(1, &m_framebufferObject);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebufferObject);
    glFramebufferTexture(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_textureName, 0);

    // check framebuffer status
    GLenum status = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE){
        spdlog::error("ImageBuffer: Framebuffer object not complete!");
        return false;
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    if(status != GL_FRAMEBUFFER_COMPLETE){
        throw std::runtime_error("ImageBuffer: Unable to construct object");
    }   
    return true;
}

auto ImageBuffer::destroy() -> void
{
    if (m_framebufferObject) {
        glDeleteFramebuffers(1, &m_framebufferObject);
        m_framebufferObject = 0;
    }
    if (m_textureName) {
        glDeleteTextures(1, &m_textureName);
        m_textureName = 0;
    }
}

auto ImageBuffer::setPixel(glm::ivec2 pos, glm::vec3 colour) -> void
{
    int idx = pos.y * m_width + pos.x;
    m_imageData[idx] = colour;

    m_modified = true;
    m_modifiedLower = std::min(m_modifiedLower, pos.y);
    m_modifiedUpper = std::max(m_modifiedUpper, pos.y + 1);
};

auto ImageBuffer::render() -> void
{
    if(!m_framebufferObject){
        spdlog::warn("ImageBuffer: Trying to render a not initialized image");
        return;
    }

    if(m_modified){
        const auto sizeY = m_modifiedUpper - m_modifiedLower;
        const auto idx = m_modifiedLower * m_width;

        glBindTexture(GL_TEXTURE_RECTANGLE, m_textureName);
        glTexSubImage2D(GL_TEXTURE_RECTANGLE, 0, 0, m_modifiedLower, m_width, sizeY, GL_RGB, GL_FLOAT, &m_imageData[idx]);
        glBindTexture(GL_TEXTURE_RECTANGLE, 0);

        resetModified();
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebufferObject);
    glBlitFramebuffer(0, 0, m_width, m_height,
                      0, 0, m_width, m_height,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
    
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
};

auto ImageBuffer::saveToFile(std::string_view imgFileName) -> bool
{
    if(m_width == 0 || m_height == 0){
        spdlog::error("ImageBuffer: Trying to save unitialized image!");
        return false;
    }

    spdlog::info("ImageBuffer: Saving image to {}...", imgFileName);

    const auto num_components = 3;
    auto pixels = std::vector<unsigned char>();
    pixels.reserve(m_height * m_height * num_components);

    for(auto y = 0; y < m_height; y++){
        for(auto x = 0; x < m_width; x++){
            auto& colour = m_imageData[y * m_width + x];
            const auto i = ((m_height - 1 - y) * m_width + x) * num_components;

            pixels[i] = (unsigned char)(255 * std::clamp(colour.r, 0.0f, 1.0f));
            pixels[i + 1] = (unsigned char)(255 * std::clamp(colour.g, 0.0f, 1.0f));
            pixels[i + 2] = (unsigned char)(255 * std::clamp(colour.b, 0.0f, 1.0f));
        }
    }

    auto stride = 0;
    if(!stbi_write_png(imgFileName.data(), m_width, m_height, num_components, pixels.data(), stride))
    {
        spdlog::error("Failed to write png file {}", imgFileName);
        return false;
    }
    return true;
};

void ImageBuffer::resetModified() 
{
    m_modified = false;
    m_modifiedLower = m_height;
    m_modifiedUpper = 0;
}
