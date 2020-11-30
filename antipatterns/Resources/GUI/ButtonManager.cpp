//
// Created by dmitry on 3/29/20.
//

#include "ButtonManager.h"
#include "Button.h"

gui::ButtonManager::ButtonManager(const std::string &params_file,
                                  const std::string &font_file) {
  LoadFont(font_file);
  InitButtons(params_file);
}

void gui::ButtonManager::LoadFont(const std::string &file_name) {
  if (!button_font_.loadFromFile(file_name)) {
    throw std::runtime_error("Cannot load font");
  }
}

void gui::ButtonManager::InitButtons(const std::string &file_name) {
  std::fstream in(file_name);
  const std::vector<Json::Node> buttons_nodes =
      Json::Load(file_name).GetRoot().AsArray();
  for (const auto &settings_map_node : buttons_nodes) {
    const auto &button_settings = settings_map_node.AsMap();
    gui::ButtonParams params{};
    std::string button_name = button_settings.at("button_name").AsString();
    params.x = button_settings.at("button_pos").AsArray().at(0).AsFloat();
    params.y = button_settings.at("button_pos").AsArray().at(1).AsFloat();
    params.width = button_settings.at("button_rect").AsArray().at(0).AsFloat();
    params.height = button_settings.at("button_rect").AsArray().at(1).AsFloat();
    params.button_text = button_settings.at("button_text").AsString();
    params.font_size = button_settings.at("font_size").AsInt();
    params.font = &button_font_;
    params.btn_colors.idle =
        ParseColor(button_settings.at("button_color_idle").AsArray());
    params.btn_colors.hover =
        ParseColor(button_settings.at("button_color_hover").AsArray());
    params.btn_colors.active =
        ParseColor(button_settings.at("button_color_active").AsArray());
    params.font_colors.idle =
        ParseColor(button_settings.at("font_color_idle").AsArray());
    params.font_colors.hover =
        ParseColor(button_settings.at("font_color_hover").AsArray());
    params.font_colors.active =
        ParseColor(button_settings.at("font_color_active").AsArray());
    buttons_[button_name] = std::make_shared<gui::Button>(params);
  }
}

bool gui::ButtonManager::IsActive(const std::string &button_name) const {
  return buttons_.at(button_name)->IsActive();
}

void gui::ButtonManager::Update(const sf::Vector2f &mouse_pos) {
  for (const auto &[str, btn_ptr] : buttons_) {
    btn_ptr->Update(mouse_pos);
  }
}

void gui::ButtonManager::Render(sf::RenderTarget &target) const {
  for (auto &[str, btn_ptr] : buttons_) {
    btn_ptr->Render(target);
  }
}

const sf::Font &gui::ButtonManager::GetFont() { return button_font_; }

std::shared_ptr<gui::Button>
    gui::ButtonManager::operator[](const std::string &btn_name) const {
  return buttons_.at(btn_name);
}

std::unordered_map<std::string, std::shared_ptr<gui::Button>>::iterator
gui::ButtonManager::begin() {
  return buttons_.begin();
}

std::unordered_map<std::string, std::shared_ptr<gui::Button>>::iterator
gui::ButtonManager::end() {
  return buttons_.end();
}
