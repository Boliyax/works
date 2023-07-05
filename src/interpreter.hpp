#ifndef INTERPRETER
#define INTERPRETER

#include "graphic_primitives_builder.hpp"
#include <algorithm>
#include <memory>
#include <sstream>
#include <stdexcept>


using Description = GraphicPrimitive::PrimitiveDescription;

std::string b_t_variant_to_string(const BasicTypesVariant &variant);

std::unique_ptr<GraphicPrimitiveBuilder>
builder_from_primitive_type(const std::string &primitive_type);

std::unique_ptr<GraphicPrimitiveBuilder>
string_to_builder(const std::string &str);

inline std::unique_ptr<GraphicPrimitive>
string_to_primitive(const std::string &str) {
  auto builder = string_to_builder(str);
  return builder->build();
}

std::string primitive_to_string(const GraphicPrimitive &primitive);

#endif