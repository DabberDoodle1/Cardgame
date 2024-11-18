#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <glad/glad.h>

#include "../include/shader.hpp"
#include "../include/texture.hpp"

class ResourceManager {
public:
  //@ Shader methods @//
  static void make_shader(const char* vertex_path, const char* fragment_path, std::string name);
  static Shader& get_shader(std::string name);

  //@ Texture methods @//
  static void make_texture(const char* texture_path, bool alpha, std::string name);
  static Texture& get_texture(std::string name);

  //@ Delete stored Shaders and Textures @//
  static void clear();

private:
  //@ Stored Shaders and Textures @//
  static std::map<std::string, Shader> Shaders;
  static std::map<std::string, Texture> Textures;

  //@ Private constructor to prevent object creation @//
  ResourceManager();
};

#endif
