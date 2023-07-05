#include "canvas.hpp"
#include <iostream>

// Заглушка отрисовки холста
Raster Canvas::generate_raster(const SizePoint &panel_size) const {
  std::cout << "Canvas rendering called" << std::endl;
  for (const auto &i : content) {
    i.second->generate_raster(RealType(panel_size.x) / RealType(width_height.x),
                              RealType(panel_size.y) /
                                  RealType(width_height.y));
  }
  return Raster{};
}