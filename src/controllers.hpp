#ifndef CONTROLLERS
#define CONTROLLERS

#include "file_system.hpp"
#include <memory>
#include <type_traits>

/**
 * @defgroup Controllers
 * @brief Контроллеры предоставляющие интерфейс работы с моделью.
 *
 * @{
 */
class GeneralController;

/// @brief Контроллер настройки и создания фигур.
class BuildController : public std::enable_shared_from_this<BuildController> {
public:
  BuildController(std::unique_ptr<GraphicPrimitiveBuilder> &&builder_,
                  std::shared_ptr<GeneralController> controller_)
      : builder(std::move(builder_)), controller(controller_) {
    for (auto &i : builder->get_interface()) {
      (this->*(get_fields_interface(i.param_type))).emplace_back(i);
    }
  }

  inline void create(const std::string &);

  template <BasicType T> size_t get_params_quantity() {
    return (this->*(get_fields_interface<T>())).size();
  }

  enum class AddingPointsStatus { not_enough, enough, filled };

  Point get_primitive_position() {
    return std::get<Point>(builder->get_interface().at(0).getter());
  }

  /// @brief Контроллер построения фигуры при помощи добавления точек.
  class AddingPointsController {
  public:
    AddingPointsController(const std::shared_ptr<BuildController> &controller_)
        : controller(controller_) {}
    AddingPointsStatus add_point(const Point &point) {
      return controller->add_point(point);
    }

  private:
    std::shared_ptr<BuildController> controller;
  };

  /// @brief Контроллер настройки параметра строителя.
  /// @tparam T Тип параметра.
  template <typename T> class FieldController {
  public:
    FieldController(const std::shared_ptr<BuildController> controller_,
                    size_t index_)
        : controller(controller_), index(index_) {}
    T get() {
      return std::get<T>(
          ((*controller).*(BuildController::get_fields_interface<T>()))
              .at(index)
              .getter());
    }
    void set(T object) {
      (((*controller).*(BuildController::get_fields_interface<T>()))
           .at(index)
           .setter(object));
    }

  private:
    std::shared_ptr<BuildController> controller;
    size_t index;
  };

  /// @brief Контролер для настройки отдельной точки фигуры.
  using PointFieldController = FieldController<Point>;
  /// @brief Контролер для настройки отдельного цвета фигуры.
  using ColorFieldController = FieldController<ColorRGBA>;
  /// @brief Контролер для настройки отдельного интегрального параметра фигуры.
  using IntegralFieldController = FieldController<IntegralType>;
  /// @brief Контролер для настройки отдельного вещественного параметра фигуры.
  using RealFieldController = FieldController<RealType>;

  template <BasicType T> BasicTypes get_param_type(size_t index) {
    return (this->*(get_fields_interface<T>())).at(index).param_type;
  }

  template <BasicType T> std::string get_param_name(size_t index) {
    return (this->*(get_fields_interface<T>())).at(index).param_name;
  }

  template <BasicType T>
  std::shared_ptr<FieldController<T>> get_field_controller(size_t index) {
    return std::make_shared<FieldController<T>>(shared_from_this(), index);
  }

  std::shared_ptr<AddingPointsController> get_adding_points_controller() {
    return std::make_shared<AddingPointsController>(shared_from_this());
  }

private:
  GraphicPrimitiveBuilder::BuilderInterface points_accessors;
  GraphicPrimitiveBuilder::BuilderInterface colors_accessors;
  GraphicPrimitiveBuilder::BuilderInterface integrals_accessors;
  GraphicPrimitiveBuilder::BuilderInterface reals_accessors;
  std::shared_ptr<GraphicPrimitiveBuilder> builder;
  std::shared_ptr<GeneralController> controller;
  size_t points_counter = 0;

  AddingPointsStatus add_point(Point point) {
    if (points_counter < points_accessors.size()) {
      if (points_counter > 0)
        point -= get_primitive_position(); // Позиция точек хранится
                                           // относительно главной точки
      points_accessors[points_counter++].setter(point);
    } else {
      points_accessors.emplace_back(builder->point_expanse());
      return add_point(point);
    }
    if (points_counter < points_accessors.size()) {
      return AddingPointsStatus::not_enough;
    } else if (builder->can_be_points_expanded()) {
      return AddingPointsStatus::enough;
    } else {
      return AddingPointsStatus::filled;
    }
  }

  static constexpr GraphicPrimitiveBuilder::BuilderInterface BuildController::*
  get_fields_interface(BasicTypes basic_enum) {
    switch (basic_enum) {
    case (BasicTypes::point): {
      return &BuildController::points_accessors;
    }
    case (BasicTypes::color_rgba): {
      return &BuildController::colors_accessors;
    }
    case (BasicTypes::integral_type): {
      return &BuildController::integrals_accessors;
    }
    default: {
      return &BuildController::reals_accessors;
    }
    }
  }
  template <BasicType T>
  static constexpr GraphicPrimitiveBuilder::BuilderInterface BuildController::*
  get_fields_interface() {
    return get_fields_interface(enum_from_basic_type<T>::value);
  }
};

/// @brief Контроллер загрузки, сохранения и создания холста.
class SaveLoadController {
public:
  inline SaveLoadController(std::shared_ptr<GeneralController>);

  inline void save(const std::string &);
  inline void load(const std::string &);
  inline void new_canvas(const SizePoint &size_vector);

private:
  std::shared_ptr<GeneralController> controller;
};

/// @brief Контроллер холста, отвечающий за удаление фигур и отрисовку.
class CanvasController {
public:
  inline CanvasController(std::shared_ptr<GeneralController>);
  inline Raster rasterize(SizePoint);
  inline void erase(const std::string &);

private:
  std::shared_ptr<GeneralController> controller;
};

/// @brief Контроллер дающий доступ к остальным контроллерам.
class GeneralController
    : public std::enable_shared_from_this<GeneralController> {
public:
  GeneralController() : canvas(std::make_unique<Canvas>()) {}

  std::shared_ptr<BuildController>
  get_build_controller(const std::string &builder_name) {
    return std::make_shared<BuildController>(
        builder_from_primitive_type(builder_name), shared_from_this());
  }

  std::shared_ptr<SaveLoadController> get_save_load_controller() {
    return std::make_shared<SaveLoadController>(shared_from_this());
  }

  std::shared_ptr<CanvasController> get_canvas_controller() {
    return std::make_shared<CanvasController>(shared_from_this());
  }

  std::unique_ptr<Canvas> canvas;
};

inline SaveLoadController::SaveLoadController(
    std::shared_ptr<GeneralController> ptr)
    : controller(ptr) {}
inline void SaveLoadController::save(const std::string &filename) {
  upload(filename, *controller->canvas);
}
inline void SaveLoadController::load(const std::string &filename) {
  controller->canvas = std::make_unique<Canvas>(download(filename));
}
inline void SaveLoadController::new_canvas(const SizePoint &size_vector) {
  controller->canvas = std::make_unique<Canvas>(size_vector);
}

inline void BuildController::create(const std::string &name) {
  controller->canvas->add_primitive(builder->build(), name);
}

inline CanvasController::CanvasController(
    std::shared_ptr<GeneralController> ptr)
    : controller(ptr) {}
inline Raster CanvasController::rasterize(SizePoint size_vector) {
  return controller->canvas->generate_raster(size_vector);
}
inline void CanvasController::erase(const std::string &name) {
  return controller->canvas->delete_primitive(name);
}

/// @}

#endif