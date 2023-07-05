#ifndef GRAPHIC_PRIMITIVES
#define GRAPHIC_PRIMITIVES

#include "basic_elements.hpp"
#include <map>
#include <stdexcept>
#include <string>


/**
 * @defgroup Primitives
 * @brief Графические примитивы.
 *
 * @{
 */

/// @brief Базовый графический примитив.
class GraphicPrimitive {
public:
  using SerializeType = std::map<const std::string, BasicTypesVariant>;

  /// @brief Универсальное описание фигуры.
  struct PrimitiveDescription {
    std::string title;
    SerializeType fields;
  };

  virtual SerializeType serialize() const;

  virtual std::string get_type() const { return "base"; };

  PrimitiveDescription get_description() const {
    return PrimitiveDescription(get_type(), serialize());
  }

  GraphicPrimitive(Point position_ = Point(), IntegralType width_ = 0,
                   ColorRGBA stroke_color_ = ColorRGBA(),
                   ColorRGBA fill_color_ = ColorRGBA(), Radians angle_ = 0)
      : position(position_), width(width_), stroke_color(stroke_color_),
        fill_color(fill_color_), angle(angle_) {
    if ((width < 0) || (width > MAX_LIMIT)) {
      throw std::range_error("Exceeding the range for coordinates");
    }
  }

  GraphicPrimitive(const GraphicPrimitive &) = default;
  GraphicPrimitive(GraphicPrimitive &&) = default;
  GraphicPrimitive &operator=(const GraphicPrimitive &) = default;
  GraphicPrimitive &operator=(GraphicPrimitive &&) = default;

  virtual ~GraphicPrimitive() = default;

  Point get_position() const noexcept { return position; };
  ColorRGBA get_stroke_color() const noexcept { return stroke_color; };
  ColorRGBA get_fill_color() const noexcept { return stroke_color; };
  Radians get_angle() const noexcept { return angle; };
  IntegralType get_width() const noexcept { return width; };

  void set_position(Point point) { position = point; };
  void set_stroke_color(ColorRGBA color) { stroke_color = color; };
  void set_fill_color(ColorRGBA color) { fill_color = color; };
  void set_angle(Radians angle_) { angle = angle_; };
  void set_width(IntegralType width_) { width = width_; };

  virtual RelativeRaster generate_raster(RealType scale_width,
                                         RealType scale_height) const;

private:
  Point position;
  IntegralType width;
  ColorRGBA stroke_color;
  ColorRGBA fill_color;
  Radians angle;
};

/// @brief Линия
class Line : public GraphicPrimitive {
public:
  SerializeType serialize() const override;
  std::string get_type() const override { return "line"; };
  Line(GraphicPrimitive base, Point point)
      : GraphicPrimitive(base), end_point(point) {}
  RelativeRaster generate_raster(RealType scale_width,
                                 RealType scale_height) const override;

private:
  Point end_point;
};

/// @brief Круг.
class Circle : public GraphicPrimitive {
public:
  SerializeType serialize() const override;
  std::string get_type() const override { return "circle"; };
  Circle(GraphicPrimitive base, IntegralType radius_)
      : GraphicPrimitive(base), radius(radius_) {
    if ((radius < 0) || (radius > MAX_LIMIT)) {
      throw std::range_error("Exceeding the range for coordinates");
    }
  }
  RelativeRaster generate_raster(RealType scale_width,
                                 RealType scale_height) const override;

private:
  IntegralType radius;
};

/// @}

#endif