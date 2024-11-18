#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture {
public:
  //@ Constructors @//
  Texture();
  Texture(unsigned int internal_format, unsigned int image_format);

  //@ Get texture ID @//
  unsigned int& get_id();

  //@ Basic essential methods @//
  void create(unsigned int width, unsigned int height, unsigned char* data);
  void bind() const;

private:
  //@ Texture ID @//
  unsigned int ID;

  //@ Texture dimensions in pixels @//
  unsigned int width;
  unsigned int height;

  //@ Texture formats @//
  unsigned int internal_format;
  unsigned int image_format;

  //@ Texture parameters @//
  unsigned int wrap_s;
  unsigned int wrap_t;
  unsigned int min_filter;
  unsigned int max_filter;
};

#endif
