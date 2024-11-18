#include "../include/texture.hpp"

Texture::Texture(): width(0), height(0), internal_format(GL_RGB), image_format(GL_RGB), wrap_s(GL_REPEAT), wrap_t(GL_REPEAT), min_filter(GL_LINEAR), max_filter(GL_LINEAR) { }

Texture::Texture(unsigned int internal_format, unsigned int image_format):
  width(0), height(0), internal_format(internal_format), image_format(image_format), wrap_s(GL_REPEAT), wrap_t(GL_REPEAT), min_filter(GL_LINEAR), max_filter(GL_LINEAR)
{
  glGenTextures(1, &this->ID);
}

void Texture::create(unsigned int width, unsigned int height, unsigned char* data)
{
  //@ Assign the height and width variable @//;w3
  this->width = width;
  this->height = height;

  //@ Bind this texture @//
  glBindTexture(GL_TEXTURE_2D, this->ID);

  //@ Create the texture and set parameters @//
  glTexImage2D(GL_TEXTURE_2D, 0, this->internal_format, width, height, 0, this->image_format, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrap_s);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrap_t);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->min_filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->max_filter);

  //@ Unbind this texture @//
  glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int& Texture::get_id()
{
  return ID;
}

void Texture::bind() const
{
  glBindTexture(GL_TEXTURE_2D, this->ID);
}
