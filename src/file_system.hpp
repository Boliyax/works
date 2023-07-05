#ifndef FILE_SYSTEM
#define FILE_SYSTEM

#include "canvas.hpp"
#include "interpreter.hpp"
#include <fstream>
#include <sstream>

void upload(const std::string &filename, const Canvas &canvas) {
  std::ofstream file(filename);
  file << canvas.get_width_height().x << ' ' << canvas.get_width_height().y
       << std::endl;
  for (const auto &i : canvas.get_content()) {
    file << i.first << ' ' << primitive_to_string(*i.second) << std::endl;
  }
}

Canvas download(const std::string &filename) {
  std::ifstream file(filename);
  std::stringstream stream;
  IntegralType width, height; // NOLINT
  file >> width >> height;
  Canvas canvas({width, height});
  std::string str;
  std::string name;

  while (file >> name) {
    std::getline(file, str);
    canvas.add_primitive(string_to_primitive(str), name);
  }
  return canvas;
}

#endif