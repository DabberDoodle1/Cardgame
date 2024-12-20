#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
  //@ Constructors @//
  Shader();
  Shader(const char* vertex_source, const char* fragment_source);

  //@ Get shader program ID @//
  unsigned int& get_id();

  //@ Basic uniform method(/s) @//
  void uniform_mat4(const char* name, glm::mat4 value) const;
  void uniform_vec4(const char* name, glm::vec4 value) const;
  void uniform_int(const char* name, int value) const;
  void uniform_float(const char* name, float value) const;

private:
  //@ Shader program ID @//
  unsigned int ID;
};

#endif
