//
// Created by dmitry on 5/5/20.
//

#ifndef ANTIPATTERNS_PROGRESSBAR_H
#define ANTIPATTERNS_PROGRESSBAR_H

#include "SimpleBar.h"
#include <cmath>

namespace gui {
struct ProgressBarParams {
  std::string bar_string;
  sf::Text text;
  float max_width;
  int max_value;
  sf::RectangleShape back;
  sf::RectangleShape inner;
};

class ProgressBar {
public:
  ProgressBar(sf::VideoMode &video_mode,
              const std::map<std::string, Json::Node> &settings,
              sf::Font *font = nullptr);

  void Update(double current_value, double max_value);

  void Render(sf::RenderTarget &target);

private:
  ProgressBarParams bar_params_;
};
} // namespace gui

#endif // ANTIPATTERNS_PROGRESSBAR_H
