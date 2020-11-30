//
// Created by dmitry on 5/5/20.
//

#ifndef ANTIPATTERNS_SIMPLEBAR_H
#define ANTIPATTERNS_SIMPLEBAR_H

#include "Button.h"

namespace gui {

class SimpleBar {
public:
  SimpleBar(sf::VideoMode &vm,
            const std::map<std::string, Json::Node> &settings);

  void SetFont(const sf::Font &font);

  void Update(int current_value);

  void Render(sf::RenderTarget &target);

private:
  std::string bar_string_;
  sf::RectangleShape back_;
  sf::Text text_;
};

} // namespace gui

#endif // ANTIPATTERNS_SIMPLEBAR_H
