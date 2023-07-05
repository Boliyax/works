#include "graphic_primitives_builder.hpp"

GraphicPrimitiveBuilder::GraphicPrimitiveBuilder() : interface {
  FieldAccessor{"position", BasicTypes::point, std::function([&](void) {
                  return BasicTypesVariant(prototype.get_position());
                }),
                std::function([&](BasicTypesVariant variant) {
                  prototype.set_position(std::get<Point>(variant));
                })

  },
      FieldAccessor{"width", BasicTypes::integral_type,
                    std::function([&](void) {
                      return BasicTypesVariant(prototype.get_width());
                    }),
                    std::function([&](BasicTypesVariant variant) {
                      prototype.set_width(std::get<IntegralType>(variant));
                    })},
      FieldAccessor{"stroke_color", BasicTypes::color_rgba,
                    std::function([&](void) {
                      return BasicTypesVariant(prototype.get_stroke_color());
                    }),
                    std::function([&](BasicTypesVariant variant) {
                      prototype.set_stroke_color(std::get<ColorRGBA>(variant));
                    })},
      FieldAccessor{"fill_color", BasicTypes::color_rgba,
                    std::function([&](void) {
                      return BasicTypesVariant(prototype.get_fill_color());
                    }),
                    std::function([&](BasicTypesVariant variant) {
                      prototype.set_fill_color(std::get<ColorRGBA>(variant));
                    })},
      FieldAccessor {
    "angle", BasicTypes::real_type, std::function([&](void) {
      return BasicTypesVariant(prototype.get_angle());
    }),
        std::function([&](BasicTypesVariant variant) {
          prototype.set_angle(std::get<RealType>(variant));
        })
  }

} {};

LineBuilder::LineBuilder() : GraphicPrimitiveBuilder() {
  add_field_accessor(FieldAccessor{
      "end_point", BasicTypes::point,
      std::function([&](void) { return BasicTypesVariant(end_point); }),
      std::function([&](BasicTypesVariant variant) {
        end_point = (std::get<Point>(variant));
      })});
}

CircleBuilder::CircleBuilder() : GraphicPrimitiveBuilder() {
  add_field_accessor(FieldAccessor{
      "radius", BasicTypes::integral_type,
      std::function([&](void) { return BasicTypesVariant(radius); }),
      std::function([&](BasicTypesVariant variant) {
        radius = (std::get<IntegralType>(variant));
      })});
}

std::unique_ptr<GraphicPrimitive> GraphicPrimitiveBuilder::build() const {
  return std::make_unique<GraphicPrimitive>(prototype);
}

std::unique_ptr<GraphicPrimitive> LineBuilder::build() const {
  return std::make_unique<Line>(get_prototype(), end_point);
}

std::unique_ptr<GraphicPrimitive> CircleBuilder::build() const {
  return std::make_unique<Circle>(get_prototype(), radius);
}