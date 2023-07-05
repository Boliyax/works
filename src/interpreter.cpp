#include "interpreter.hpp"

std::string b_t_variant_to_string(const BasicTypesVariant &variant) {
  switch (variant.index()) {
  case size_t(BasicTypes::point): {
    const Point &p = std::get<Point>(variant);
    return std::to_string(p.x) + ' ' + std::to_string(p.y);
    break;
  }
  case size_t(BasicTypes::color_rgba): {
    const ColorRGBA &c = std::get<ColorRGBA>(variant);
    return std::to_string(c.red) + ' ' + std::to_string(c.green) + ' ' +
           std::to_string(c.blue) + ' ' + std::to_string(c.alpha);
    break;
  }
  case size_t(BasicTypes::integral_type): {
    return std::to_string(std::get<IntegralType>(variant));
    break;
  }
  default: {
    return std::to_string(std::get<RealType>(variant));
  }
  }
}

std::unique_ptr<GraphicPrimitiveBuilder>
builder_from_primitive_type(const std::string &primitive_type) {
  std::unique_ptr<GraphicPrimitiveBuilder> builder_ptr;
  if (primitive_type == "base") {
    return std::make_unique<GraphicPrimitiveBuilder>();
  } else if (primitive_type == "line") {
    return std::make_unique<LineBuilder>();
  } else if (primitive_type == "circle") {
    return std::make_unique<CircleBuilder>();
  }
  throw std::logic_error("Undefined type");
}

std::unique_ptr<GraphicPrimitiveBuilder>
string_to_builder(const std::string &str) {
  std::stringstream stream;
  stream << str;
  std::string primitive_type;
  stream >> primitive_type;
  std::unique_ptr<GraphicPrimitiveBuilder> builder_ptr =
      builder_from_primitive_type(primitive_type);

  for (std::string field; stream >> field;) {
    const auto &iter =
        std::ranges::find(builder_ptr->get_interface(), field,
                          [](const FieldAccessor &x) { return x.param_name; });

    if (iter == builder_ptr->get_interface().end()) {
      throw std::logic_error("Undefined field");
    }

    switch (iter->param_type) {
    case (BasicTypes::point): {
      IntegralType x, y; // NOLINT
      stream >> x >> y;
      iter->setter(Point(x, y));
      break;
    }
    case (BasicTypes::color_rgba): {
      using Ton = ColorRGBA::Ton;
      unsigned short r, g, b, a; // NOLINT
      stream >> r >> g >> b >> a;
      iter->setter(ColorRGBA(Ton(r), Ton(g), Ton(b), Ton(a)));
      break;
    }
    case (BasicTypes::integral_type): {
      IntegralType x; // NOLINT
      stream >> x;
      iter->setter(x);
      break;
    }
    default: {
      RealType x; // NOLINT
      stream >> x;
      iter->setter(x);
      break;
    }
    }
  }
  return builder_ptr;
}

std::string primitive_to_string(const GraphicPrimitive &primitive) {
  Description description = primitive.get_description();
  std::string str = description.title + ' ';
  for (const auto &i : description.fields) {
    str += i.first + ' ' + b_t_variant_to_string(i.second) + ' ';
  }
  return str;
}
