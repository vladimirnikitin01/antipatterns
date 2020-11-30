//
// Created by dmitry on 5/5/20.
//

#include "SimpleBar.h"

gui::SimpleBar::SimpleBar(sf::VideoMode &vm,
                          const std::map<std::string, Json::Node> &settings) {
  float width = gui::ToPixelsX(settings.at("width").AsFloat(), vm);
  float height = gui::ToPixelsY(settings.at("height").AsFloat(), vm);
  float x = gui::ToPixelsX(settings.at("pos_x").AsFloat(), vm);
  float y = gui::ToPixelsY(settings.at("pos_y").AsFloat(), vm);
  back_.setSize(sf::Vector2f(width, height));
  back_.setPosition(sf::Vector2f(x, y));
  back_.setFillColor(gui::ParseColor(settings.at("fill_color").AsArray()));
  text_.setCharacterSize(
      gui::CalcCharacterSize(vm, settings.at("character_size").AsInt()));
  text_.setPosition(
      back_.getPosition().x +
          gui::ToPixelsX(settings.at("text_pos_x").AsFloat(), vm),
      back_.getPosition().y +
          gui::ToPixelsY(settings.at("text_pos_y").AsFloat(), vm));
}

void gui::SimpleBar::SetFont(const sf::Font &font) { text_.setFont(font); }

void gui::SimpleBar::Update(int current_value) {
  bar_string_ = std::to_string(current_value);
  text_.setString(bar_string_);
}

void gui::SimpleBar::Render(sf::RenderTarget &target) {
  target.draw(back_);
  target.draw(text_);
}
