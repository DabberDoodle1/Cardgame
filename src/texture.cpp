#include "../include/texture.hpp"

#include <glad/glad.h>

Texture::Texture(): format(GL_RGB) { }

Texture::Texture(unsigned int format): format(format)
{
  glGenTextures(1, &ID);
}

void Texture::create(unsigned int width, unsigned int height, unsigned char* data)
{
  // Prepare this texture for image and parameters
  glBindTexture(GL_TEXTURE_2D, ID);


  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


  if (format == GL_RED || format == GL_RG)
  {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);

    if (format == GL_RG)
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_GREEN);
  }


  // Change the unpack alignment of pixels due to images to account for images that don't have 4 channels
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


  glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int& Texture::get_id()
{
  return ID;
}

void Texture::bind() const
{
  glBindTexture(GL_TEXTURE_2D, ID);
}
