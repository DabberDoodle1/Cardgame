#ifndef SHADER_H
#define SHADER_H

class Shader {
public:
  Shader();
  Shader(const char *vertex_source, const char *fragment_source);

  unsigned int& get_id();
  void          uniform_mat4(const char *name, const float *value) const;
  void          uniform_vec4(const char *name, const float *value) const;
  void          uniform_int(const char *name, int value) const;
  void          uniform_float(const char *name, float value) const;
private:
  unsigned int  ID;
};

#endif
