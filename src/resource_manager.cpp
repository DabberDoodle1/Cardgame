#include "../include/resource_manager.hpp"
#include "../include/stb_image.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

//@ Initialize static variables @//
std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture> ResourceManager::Textures;


void ResourceManager::make_shader(const char* vertex_path, const char* fragment_path, std::string name)
{
  std::string vertex_source;
  std::string fragment_source;
  try
  {
    //@ Stream buffers to get and read file content @//
    std::ifstream fs;
    std::stringstream ss;

    //@ Open vertex shader file @//
    fs.open(vertex_path);
    if (fs.is_open())
    {
      //@ Read file contents into vertex shader source variable @//
      ss << fs.rdbuf();
      vertex_source = ss.str();
      fs.close();

      //@ Refresh stringstream buffer @//
      ss.str("");
      ss.clear();
    }
    else throw std::runtime_error("VERTEX SHADER FILE NOT FOUND!");

    //@ Open fragment shader file @//
    fs.open(fragment_path);
    if (fs.is_open())
    {
      //@ Read file contents into fragment shader source variable @//
      ss << fs.rdbuf();
      fragment_source = ss.str();
      fs.close();
    }
    else throw std::runtime_error("FRAGMENT SHADER FILE NOT FOUND!");
  }
  catch (std::exception e)
  {
    std::cout << "ERROR: " << e.what() << std::endl;
  }

  //@ Create and map the shader @//
  Shaders[name] = Shader(vertex_source.c_str(), fragment_source.c_str());
}

Shader& ResourceManager::get_shader(std::string name)
{
  return Shaders[name];
}

void ResourceManager::make_texture(const char* texture_path, bool alpha, std::string name)
{
  //@ Defining texture formats with ternary operator @//
  unsigned int internal_format = (alpha == true) ? GL_RGBA : GL_RGB;
  unsigned int image_format = (alpha == true) ? GL_RGBA : GL_RGB;

  //@ Create the texture object @//
  Texture texture(internal_format, image_format);

  //@ Loading the image data @//
  int width, height, nrChannels;
  unsigned char* data = stbi_load(texture_path, &width, &height, &nrChannels, 0);

  //@ Creating and mapping the texture  @//
  texture.create(width, height, data);
  Textures[name] = texture;

  //@ Free image data to avoid memory leaks @//
  stbi_image_free(data);
}

Texture& ResourceManager::get_texture(std::string name)
{
  return Textures[name];
}

void ResourceManager::clear()
{
  //@ Deleting stored Shaders and Textures @//
  for (auto shader : Shaders)
    glDeleteProgram(shader.second.get_id());
  for (auto texture : Textures)
    glDeleteTextures(1, &texture.second.get_id());
}