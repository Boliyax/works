#include "graphic_primitives.hpp"
#include <iostream>

GraphicPrimitive::SerializeType GraphicPrimitive::serialize() const {
  return SerializeType{
      SerializeType::value_type{SerializeType::key_type("position"),
                                SerializeType::mapped_type(position)},
      SerializeType::value_type{SerializeType::key_type("width"),
                                SerializeType::mapped_type(width)},
      SerializeType::value_type{SerializeType::key_type("stroke_color"),
                                SerializeType::mapped_type(stroke_color)},
      SerializeType::value_type{SerializeType::key_type("fill_color"),
                                SerializeType::mapped_type(fill_color)},
      SerializeType::value_type{SerializeType::key_type("angle"),
                                SerializeType::mapped_type(angle)}};
}

Line::SerializeType Line::serialize() const {
  auto map = GraphicPrimitive::serialize();
  map.insert(std::initializer_list<GraphicPrimitive::SerializeType::value_type>{
      SerializeType::value_type{SerializeType::key_type("end_point"),
                                SerializeType::mapped_type(end_point)}});
  return map;
}

Circle::SerializeType Circle::serialize() const {
  auto map = GraphicPrimitive::serialize();
  map.emplace(SerializeType::value_type{SerializeType::key_type("radius"),
                                        SerializeType::mapped_type(radius)});
  return map;
}

// Заглушка отрисовки примитива
RelativeRaster GraphicPrimitive::generate_raster(RealType width_scale,
                                                 RealType hight_scale) const {
  (void)width_scale;
  (void)hight_scale;
  std::cout << "Draw primitive" << std::endl;
  return RelativeRaster();
}

// Заглушка отрисовки линии
RelativeRaster Line::generate_raster(RealType width_scale,
                                     RealType hight_scale) const {
  (void)width_scale;
  (void)hight_scale;
  std::cout << "Draw line" << std::endl;
  return RelativeRaster();
}

// Заглушка отрисовки круга
RelativeRaster Circle::generate_raster(RealType width_scale,
                                       RealType hight_scale) const {
  (void)width_scale;
  (void)hight_scale;
  std::cout << "Draw circle" << std::endl;
  return RelativeRaster();
}
