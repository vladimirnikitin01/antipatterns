//
// Created by dmitry on 5/5/20.
//

#include "GuiFunctions.h"

float gui::ToPixelsX(float rel_value, sf::VideoMode &video_mode) {
  return rel_value / 100.f * static_cast<float>(video_mode.width);
}

float gui::ToPixelsY(float rel_value, sf::VideoMode &video_mode) {
  return rel_value / 100.f * static_cast<float>(video_mode.height);
}

unsigned int gui::CalcCharacterSize(sf::VideoMode &video_mode,
                                    unsigned int normal_size) {
  return static_cast<unsigned int>(static_cast<float>(video_mode.height) /
                                   kNormalHeight) *
         normal_size;
}

sf::Color gui::ParseColor(const std::vector<Json::Node> &color_json) {
  int r = color_json.at(0).AsInt();
  int g = color_json.at(1).AsInt();
  int b = color_json.at(2).AsInt();
  int alpha = color_json.at(3).AsInt();
  return sf::Color(r, g, b, alpha);
}
