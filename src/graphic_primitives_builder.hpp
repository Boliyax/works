#ifndef GRAPHIC_PRIMITIVES_BUILDER
#define GRAPHIC_PRIMITIVES_BUILDER

#include "graphic_primitives.hpp"
#include <functional>
#include <memory>

/**
 * @defgroup Builders
 * @brief Строители примитивов.
 *
 * @{
 */

/// @brief Структура доступа к полям настройки конструктора
struct FieldAccessor {
  std::string param_name;
  BasicTypes param_type;
  std::function<BasicTypesVariant(void)> getter;
  std::function<void(BasicTypesVariant)> setter;
};

/// @brief Строитель базового примитива
class GraphicPrimitiveBuilder {
public:
  using BuilderInterface = std::vector<FieldAccessor>;
  const BuilderInterface &get_interface() { return interface; };
  GraphicPrimitiveBuilder();
  virtual ~GraphicPrimitiveBuilder() = default;
  GraphicPrimitiveBuilder(const GraphicPrimitiveBuilder &) = delete;
  GraphicPrimitiveBuilder(GraphicPrimitiveBuilder &&) = delete;
  GraphicPrimitiveBuilder &operator=(const GraphicPrimitiveBuilder &) = delete;
  GraphicPrimitiveBuilder &operator=(GraphicPrimitiveBuilder &&) = delete;

  virtual bool can_be_points_expanded() { return false; }
  virtual FieldAccessor point_expanse() {
    throw std::logic_error("Can't be expand");
  }

  const GraphicPrimitive &get_prototype() const { return prototype; };

  virtual std::unique_ptr<GraphicPrimitive> build() const;

protected:
  void add_field_accessor(FieldAccessor field_accessor) {
    interface.push_back(field_accessor);
  }

private:
  GraphicPrimitive prototype = GraphicPrimitive();
  BuilderInterface interface;
};

/// @brief Строитель линии
class LineBuilder : public GraphicPrimitiveBuilder {
public:
  LineBuilder();

  std::unique_ptr<GraphicPrimitive> build() const override;

private:
  Point end_point;
};

/// @brief Строитель круга
class CircleBuilder : public GraphicPrimitiveBuilder {
public:
  CircleBuilder();

  std::unique_ptr<GraphicPrimitive> build() const override;

private:
  IntegralType radius;
};

using BuilderHandle = std::unique_ptr<GraphicPrimitiveBuilder>;

/// @}

#endif