#ifndef BASIC_ELEMENTS
#define BASIC_ELEMENTS

#include <stdexcept>
#include <variant>
#include <vector>

using IntegralType = int;
using RealType = float;
using Radians = RealType;
static constexpr int MAX_LIMIT = 1'000'000'000;
static constexpr int MIN_LIMIT = -MAX_LIMIT;

/// @brief Класс для обозначении точки либо вектора.
template <typename Coordinate, Coordinate MAX_LIMIT,
          Coordinate MIN_LIMIT = -MAX_LIMIT>
struct BasicPoint {
  Coordinate x = 0;
  Coordinate y = 0;

  BasicPoint() = default;

  BasicPoint(Coordinate x_, Coordinate y_) : x(x_), y(y_) {
    if ((x < MIN_LIMIT) || (x > MAX_LIMIT) || (y < MIN_LIMIT) ||
        (y > MAX_LIMIT)) {
      throw std::range_error("Exceeding the range for coordinates");
    }
  }

  BasicPoint operator-() const { return BasicPoint{-x, -y}; }

  BasicPoint &operator+=(const BasicPoint &rhs) {
    x += rhs.x;
    y += rhs.y;
    if ((x < MIN_LIMIT) || (x > MAX_LIMIT) || (y < MIN_LIMIT) ||
        (y > MAX_LIMIT)) {
      x -= rhs.x;
      y -= rhs.y;
      throw std::range_error("Exceeding the range for coordinates");
    }
    return *this;
  }
  BasicPoint &operator-=(const BasicPoint &rhs) { return *this += (-rhs); }
  BasicPoint operator+(const BasicPoint &rhs) const {
    return BasicPoint(*this) += rhs;
  }
  BasicPoint operator-(const BasicPoint &rhs) const { return *this + (-rhs); }
};
constexpr IntegralType MAX_INTEGRAL_SIZE = 1'000'000'000;
using Point = BasicPoint<IntegralType, MAX_INTEGRAL_SIZE>;
using SizePoint = BasicPoint<IntegralType, MAX_INTEGRAL_SIZE, 0>;

/// @brief Цвет в формате RGBA
struct ColorRGBA {
  using Ton = unsigned char;

  Ton red = 0;
  Ton green = 0;
  Ton blue = 0;
  Ton alpha = 0;
};

using BasicTypesVariant =
    std::variant<Point, ColorRGBA, IntegralType, RealType>;

/// @brief Перечисление основных используемых типов.
enum class BasicTypes { point, color_rgba, integral_type, real_type };

/// @brief Растровое изображение.
struct Raster {
  SizePoint SizeVector;
  std::vector<ColorRGBA> data = std::vector<ColorRGBA>();
};

/// @brief Растр с обозначением точки отсчета фигуры, для определения положения
/// положения на холсте.
struct RelativeRaster : public Raster {
  Point center = Point();
};

template <typename T>
concept BasicType =
    std::is_same_v<T, Point> || std::is_same_v<T, ColorRGBA> ||
    std::is_same_v<T, IntegralType> || std::is_same_v<T, RealType>;

template <BasicType T> struct enum_from_basic_type {};

template <> struct enum_from_basic_type<Point> {
  static constexpr BasicTypes value = BasicTypes::point;
};

template <> struct enum_from_basic_type<ColorRGBA> {
  static constexpr BasicTypes value = BasicTypes::color_rgba;
};

template <> struct enum_from_basic_type<IntegralType> {
  static constexpr BasicTypes value = BasicTypes::integral_type;
};

template <> struct enum_from_basic_type<RealType> {
  static constexpr BasicTypes value = BasicTypes::real_type;
};

template <BasicType T>
using enum_from_basic_type_v = enum_from_basic_type<T>::value;

#endif