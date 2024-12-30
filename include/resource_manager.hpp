#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include "../include/shader.hpp"
#include "../include/texture.hpp"

class ResourceManager {
public:
  static void     make_shader(const char* vertex_path, const char* fragment_path, const std::string name);
  static void     make_texture(const char* texture_path, const std::string name);
  static Shader&  get_shader(std::string nam);
  static Texture& get_texture(std::string name);
  static void     clear(); // Delete stored Shaders and Textures
private:
  // Store all shaders and textures generated in maps
  static std::map<std::string, Shader>  Shaders;
  static std::map<std::string, Texture> Textures;

  ResourceManager();
};

#endif
