//
// Created by Jack Glass on 2020-10-15.
//

#include "texture.h"

#include <utility>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

using namespace gl;

Texture::Texture(std::string path, GLint interpolation)
  : m_handle()
  , m_path(std::move(path))
  , m_interpolation(interpolation)
{
  stbi_set_flip_vertically_on_load(true);
  const auto* path_data = m_path.c_str();

  int num_components;
  auto* data = stbi_load(path_data, &m_width, &m_height, &num_components, 0);

  if(data != nullptr){
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    bind();

    auto format = GL_RGB;
    switch(num_components){
    case 4:
      format = GL_RGBA;
      break;
    case 3:
      format = GL_RGB;
      break;
    case 2:
      format = GL_RG;
      break;
    case 1:
      format = GL_RED;
      break;
    default:
      std::cout << "Invalid Texture Format\n";
      break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0 , format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolation);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolation);

    unbind();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    stbi_image_free(data);
  } else {
    throw std::runtime_error("Failed to read texture data from file!");
  }
}

auto Texture::bind() const -> void
{
  glBindTexture(GL_TEXTURE_2D, m_handle);
}

auto Texture::unbind() const -> void
{
//  glBindTexture(GL_TEXTURE_2D, 0); //Bind to something else?
}