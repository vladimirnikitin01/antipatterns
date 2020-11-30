//
// Created by dmitry on 5/5/20.
//

#include "ProgressBar.h"

gui::ProgressBar::ProgressBar(sf::VideoMode &video_mode,
                              const std::map<std::string, Json::Node> &settings,
                              sf::Font *font) {
  float width = gui::ToPixelsX(settings.at("width").AsFloat(), video_mode);
  float height = gui::ToPixelsY(settings.at("height").AsFloat(), video_mode);
  float pos_x = gui::ToPixelsX(settings.at("pos_x").AsFloat(), video_mode);
  float pos_y = gui::ToPixelsY(settings.at("pos_y").AsFloat(), video_mode);

  bar_params_.max_width = width;
  bar_params_.back.setSize(sf::Vector2f(width, height));
  bar_params_.back.setFillColor(
      gui::ParseColor(settings.at("back_fill_color").AsArray()));
  bar_params_.back.setPosition(pos_x, pos_y);

  bar_params_.inner.setSize(sf::Vector2f(width, height));
  bar_params_.inner.setFillColor(
      gui::ParseColor(settings.at("inner_fill_color").AsArray()));
  bar_params_.inner.setPosition(pos_x, pos_y);

  if (font != nullptr) {
    bar_params_.text.setFont(*font);
    bar_params_.text.setCharacterSize(gui::CalcCharacterSize(
        video_mode, settings.at("character_size").AsInt()));
    bar_params_.text.setPosition(
        bar_params_.inner.getPosition().x +
            gui::ToPixelsX(settings.at("text_pos_x").AsFloat(), video_mode),
        bar_params_.inner.getPosition().y +
            gui::ToPixelsY(settings.at("text_pos_y").AsFloat(), video_mode));
  }
}

void gui::ProgressBar::Update(double current_value, double max_value) {
  if (bar_params_.max_value != max_value) {
    bar_params_.max_value = static_cast<int>(std::round(max_value));
  }
  float curr_percent = static_cast<float>(current_value) /
                       static_cast<float>(bar_params_.max_value);
  bar_params_.inner.setSize(
      sf::Vector2f(std::floor(bar_params_.max_width * curr_percent),
                   bar_params_.inner.getSize().y));
  bar_params_.bar_string =
      std::to_string(static_cast<int>(std::round(current_value))) + " / " +
      std::to_string(bar_params_.max_value);
  bar_params_.text.setString(bar_params_.bar_string);
}

void gui::ProgressBar::Render(sf::RenderTarget &target) {
  target.draw(bar_params_.back);
  target.draw(bar_params_.inner);
  target.draw(bar_params_.text);
}
