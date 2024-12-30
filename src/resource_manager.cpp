#include "../include/resource_manager.hpp"
#include "../include/stb_image.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>

using std::map;
using std::string;
using std::ifstream;
using std::stringstream;

map<string, Shader> ResourceManager::Shaders;
map<string, Texture> ResourceManager::Textures;

void ResourceManager::make_shader(const char* vertex_path, const char* fragment_path, const string name)
{
  string vertex_source;
  string fragment_source;

  ifstream filestream;
  stringstream stringstream;

  // Open vertex shader file, readbuffer into string stream, copy stringstream content to vertex_source
  filestream.open(vertex_path);
  stringstream << filestream.rdbuf();
  vertex_source = stringstream.str();
  filestream.close();

  // clear stringstream content and clear stringstream flags to avoid errors when reading filestream buffer again
  stringstream.str("");
  stringstream.clear();

  // Open fragment shader file, readbuffer into string stream, copy stringstream content to fragment_source
  filestream.open(fragment_path);
  stringstream << filestream.rdbuf();
  fragment_source = stringstream.str();
  filestream.close();

  // Create and map the shader
  Shaders[name] = Shader(vertex_source.c_str(), fragment_source.c_str());
}

void ResourceManager::make_texture(const char* texture_path, const string name)
{
  // Loading the image data
  int width, height, nrChannels;
  unsigned char* data = stbi_load(texture_path, &width, &height, &nrChannels, 0);

  // Assigning texture formats depending on normal channels
  unsigned int format;

  switch (nrChannels)
  {
    case 1:
      format = GL_RED;
      break;
    case 2:
      format = GL_RG;
      break;
    case 3:
      format = GL_RGB;
      break;
    case 4:
      format = GL_RGBA;
      break;
  }

  // Create a new texture and store it
  Texture texture(format);
  texture.create(width, height, data);
  Textures[name] = texture;

  // Free image data to avoid memory leaks
  stbi_image_free(data);
}

Shader& ResourceManager::get_shader(string name)
{
  return Shaders[name];
}

Texture& ResourceManager::get_texture(string name)
{
  return Textures[name];
}

void ResourceManager::clear()
{
  for (auto shader : Shaders)
    glDeleteProgram(shader.second.get_id());
  for (auto texture : Textures)
    glDeleteTextures(1, &texture.second.get_id());
}
