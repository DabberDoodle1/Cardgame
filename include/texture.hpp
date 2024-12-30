#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
public:
  Texture();
  Texture(unsigned int format);

  unsigned int& get_id();
  void          create(unsigned int width, unsigned int height, unsigned char* data);
  void          bind() const;
private:
  unsigned int  ID;
  unsigned int  format;
};

#endif
