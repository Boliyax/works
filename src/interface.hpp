
#ifndef INTERFACE
#define INTERFACE
#include "controllers.hpp"
#include <climits>


constexpr IntegralType INTEGRAL_SLIDER_MAX = 1000;

/**
 * @defgroup Interface
 * @brief Классы интерфейса пользователя.
 *
 * @{
 */

/// @brief Действие принимаемое панелью.
class Action {
public:
  unsigned char number;
};

/// @brief Класс каркас панели.
struct ScreenPanelFrame {
  Point position = Point();
  SizePoint size_vector = SizePoint();
};

/// @brief Панель для взаимодействия с пользователем
class ActiveScreenPanel : public ScreenPanelFrame { // NOLINT
public:
  virtual Raster draw() { return Raster(); } // Заглушка
  virtual void some_action(const Action &action) { (void)action; } // Заглушка
  virtual ~ActiveScreenPanel() = default;
};

/// @brief Слайдер для настройки параметров.
class Slider : public ActiveScreenPanel {
public:
  Raster draw() override { return Raster(); } // Заглушка
  Slider(IntegralType min_, IntegralType max_)
      : min(min_), max(max_), current(min_) {}
  void some_action(const Action &action) override { (void)action; } // Заглушка
  IntegralType get_current() { return current; }
  void set_current(IntegralType value) { current = value; }

private:
  IntegralType min;
  IntegralType max;
  IntegralType current;
};

/// @brief Панель для настройки параметров строителя.
class FieldSettingPanel : public ActiveScreenPanel {
  virtual void fixing_values() = 0;
};

/// @brief Панель для настройки точки.
class PointSettingPanel : public FieldSettingPanel {
public:
  Raster draw() override { return Raster(); } // Заглушка
  PointSettingPanel(
      const std::shared_ptr<BuildController::FieldController<Point>>
          &controller_)
      : controller(controller_) {}
  void some_action(const Action &action) override { (void)action; } // Заглушка
  void fixing_values() override {}
  std::shared_ptr<BuildController::FieldController<Point>> controller;
};

/// @brief Панель для настройки цвета.
class ColorSettingPanel : public FieldSettingPanel {
public:
  Raster draw() override { return Raster(); } // Заглушка
  ColorSettingPanel(
      const std::shared_ptr<BuildController::FieldController<ColorRGBA>>
          &controller_)
      : controller(controller_) {}
  void some_action(const Action &action) override { (void)action; } // Заглушка
  void fixing_values() override;
  std::shared_ptr<BuildController::FieldController<ColorRGBA>> controller;
  Slider r = Slider(0, UCHAR_MAX);
  Slider g = Slider(0, UCHAR_MAX);
  Slider b = Slider(0, UCHAR_MAX);
  Slider a = Slider(0, UCHAR_MAX);
};

/// @brief Панель для настройки интегральных параметров.
class IntegralSettingPanel : public FieldSettingPanel {
public:
  Raster draw() override { return Raster(); } // Заглушка
  IntegralSettingPanel(
      const std::shared_ptr<BuildController::FieldController<IntegralType>>
          &controller_)
      : controller(controller_) {}
  void some_action(const Action &action) override { (void)action; } // Заглушка
  void fixing_values() override;
  std::shared_ptr<BuildController::FieldController<IntegralType>> controller;
  Slider slider = Slider(0, INTEGRAL_SLIDER_MAX);
};

/// @brief Панель для настройки вещественных параметров.
class RealSettingPanel : public FieldSettingPanel {
public:
  Raster draw() override { return Raster(); } // Заглушка
  RealSettingPanel(
      const std::shared_ptr<BuildController::FieldController<RealType>>
          &controller_)
      : controller(controller_) {}
  void some_action(const Action &action) override { (void)action; } // Заглушка
  void fixing_values() override {}
  std::shared_ptr<BuildController::FieldController<RealType>> controller;
};

/// @brief Панель для взаимодействия с холстом.
class CanvasPanel : public ActiveScreenPanel {
public:
  Raster draw() override { return canvas_controller->rasterize(size_vector); }
  CanvasPanel(const std::shared_ptr<CanvasController> &c_controller,
              const std::shared_ptr<BuildController::AddingPointsController>
                  &ap_controller)
      : canvas_controller(c_controller), add_point_controller(ap_controller) {}
  void some_action(const Action &action) override { (void)action; } // Заглушка
  void local_cords_click(const Point &);
  void delete_selected_primitive();
  void select_primitive() {}
  std::string selected_primitive;
  std::shared_ptr<CanvasController> canvas_controller;
  std::shared_ptr<BuildController::AddingPointsController> add_point_controller;
  BuildController::AddingPointsStatus status =
      BuildController::AddingPointsStatus::not_enough;
};

/// @brief Панель для загрузки, сохранения и создания нового холста.
class SaveLoadPanel : public ActiveScreenPanel {
public:
  Raster draw() override { return Raster(); } // Заглушка
  SaveLoadPanel(const std::shared_ptr<SaveLoadController> &controller_)
      : controller(controller_) {}
  void some_action(const Action &action) override { (void)action; } // Заглушка
  void press_save();
  void press_load();
  void press_new();
  std::shared_ptr<SaveLoadController> controller;
  SizePoint canvas_generate_size;
  std::string path;
};

/// @brief Панель выбора конструируемой фигуры.
class BuilderPanel : public ActiveScreenPanel {
public:
  BuilderPanel(const std::shared_ptr<GeneralController> controller_)
      : controller(controller_) {}
  void select_builder(const std::string &builder_name) {
    curent_builder_controller = controller->get_build_controller(builder_name);
  }
  void configuring(ActiveScreenPanel &build_window, CanvasPanel &canvas) {
    (void)build_window;
    (void)canvas;
  } // настройка окружения под новый конструктор.Не реализованно.
  void press_create();
  std::string primitive_name;
  std::shared_ptr<GeneralController> controller;
  std::shared_ptr<BuildController> curent_builder_controller;
};

/// @}

#endif