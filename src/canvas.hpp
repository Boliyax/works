#ifndef CANVAS
#define CANVAS

#include "graphic_primitives.hpp"
#include <memory>


constexpr IntegralType DEFAULT_CANVAS_WIDTH = 100;
constexpr IntegralType DEFAULT_CANVAS_HEIGHT = 100;

/// @brief Холст, хранит примитивы, и отрисовывает изображение по запросу.
class Canvas {
public:
  using ID = const std::string;
  using PrimitiveHandle = std::unique_ptr<GraphicPrimitive>;
  Raster generate_raster(const SizePoint &) const;
  Canvas() = default;
  Canvas(const SizePoint &size_vector) : width_height(size_vector) {}
  // void add_primitive(PrimitiveHandler&& primitive_ptr); // Автоматическая
  // генерация ID фигур не реализована
  void add_primitive(PrimitiveHandle &&primitive_ptr, const ID &id) {
    content.emplace(id, std::move(primitive_ptr));
  }
  void delete_primitive(const ID &id) { content.erase(id); }

  const std::map<ID, PrimitiveHandle> &get_content() const { return content; }
  const SizePoint &get_width_height() const { return width_height; }

private:
  SizePoint width_height =
      SizePoint(DEFAULT_CANVAS_WIDTH, DEFAULT_CANVAS_HEIGHT);
  std::map<ID, PrimitiveHandle> content = std::map<ID, PrimitiveHandle>();
};

using CanvasHandle = std::unique_ptr<Canvas>;

#endif