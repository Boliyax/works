#include "interface.hpp"
#include <iostream>

void SaveLoadPanel::press_save() { controller->save(path); }
void SaveLoadPanel::press_load() { controller->load(path); }
void SaveLoadPanel::press_new() {
  controller->new_canvas(canvas_generate_size);
}

void CanvasPanel::local_cords_click(const Point &point) {
  if (status != BuildController::AddingPointsStatus::filled)
    status = add_point_controller->add_point(point);
}
void CanvasPanel::delete_selected_primitive() {
  canvas_controller->erase(selected_primitive);
}

void IntegralSettingPanel::fixing_values() {
  controller->set(slider.get_current());
}

void ColorSettingPanel::fixing_values() {
  controller->set({static_cast<unsigned char>(r.get_current()),
                   static_cast<unsigned char>(g.get_current()),
                   static_cast<unsigned char>(b.get_current()),
                   static_cast<unsigned char>(a.get_current())});
}

void BuilderPanel::press_create() {
  curent_builder_controller->create(primitive_name);
}

// За неимением графического интерфейса, изменения в объекты интерфейса вносятся
// напрямую.
int main() {

  // Следующий код симулирует обращение к графическому интерфейсу приложения.
  // NOLINTBEGIN
  // инициализация рабочего окна
  auto controller = std::make_shared<GeneralController>();
  auto build_panel = BuilderPanel(controller);
  build_panel.select_builder("line");
  auto save_load_panel = SaveLoadPanel(controller->get_save_load_controller());
  auto canvas = CanvasPanel(
      controller->get_canvas_controller(),
      build_panel.curent_builder_controller->get_adding_points_controller());
  {
    // Создание панелей настройки примитива
    auto color_panel_0 = ColorSettingPanel(
        build_panel.curent_builder_controller->get_field_controller<ColorRGBA>(
            0));
    auto color_panel_1 = ColorSettingPanel(
        build_panel.curent_builder_controller->get_field_controller<ColorRGBA>(
            1));
    auto width_panel =
        IntegralSettingPanel(build_panel.curent_builder_controller
                                 ->get_field_controller<IntegralType>(0));

    // создание нового окна
    save_load_panel.canvas_generate_size = {400, 300};
    save_load_panel.press_new();

    // Установка толщины линии
    width_panel.slider.set_current(4);
    width_panel.fixing_values();

    // Установка ползунков цвета
    color_panel_0.r.set_current(100);
    color_panel_0.g.set_current(200);
    color_panel_0.b.set_current(0);
    color_panel_0.a.set_current(255);
    color_panel_0.fixing_values();

    color_panel_1.r.set_current(0);
    color_panel_1.g.set_current(0);
    color_panel_1.b.set_current(0);
    color_panel_1.a.set_current(255);
    color_panel_1.fixing_values();

    // Симуляция выставления точек на холсте
    canvas.local_cords_click({50, 50});
    canvas.local_cords_click({60, 70});

    // Выбор идентификатора и создание линии
    build_panel.primitive_name = "my_line";
    build_panel.press_create();

    // Отрисовка
    canvas.draw();

    // Сохранение
    save_load_panel.path = "output.txt";
    save_load_panel.press_save();
  }
  {
    build_panel.select_builder("circle");

    auto color_panel_0 = ColorSettingPanel(
        build_panel.curent_builder_controller->get_field_controller<ColorRGBA>(
            0));
    auto color_panel_1 = ColorSettingPanel(
        build_panel.curent_builder_controller->get_field_controller<ColorRGBA>(
            1));
    auto width_panel =
        IntegralSettingPanel(build_panel.curent_builder_controller
                                 ->get_field_controller<IntegralType>(0));
    auto radius_panel =
        IntegralSettingPanel(build_panel.curent_builder_controller
                                 ->get_field_controller<IntegralType>(1));

    width_panel.slider.set_current(4);
    width_panel.fixing_values();

    radius_panel.slider.set_current(30);
    radius_panel.fixing_values();

    color_panel_0.r.set_current(127);
    color_panel_0.g.set_current(127);
    color_panel_0.b.set_current(127);
    color_panel_0.a.set_current(200);
    color_panel_0.fixing_values();

    color_panel_1.r.set_current(0);
    color_panel_1.g.set_current(0);
    color_panel_1.b.set_current(255);
    color_panel_1.a.set_current(255);
    color_panel_1.fixing_values();

    canvas.local_cords_click({78, 25});

    build_panel.primitive_name = "my_circle";
    build_panel.press_create();

    canvas.draw();

    save_load_panel.path = "other_output.txt";
    save_load_panel.press_save();
  }
  {
    save_load_panel.path = "output.txt";
    save_load_panel.press_load();
    canvas.draw();
  }
  // NOLINTEND
}