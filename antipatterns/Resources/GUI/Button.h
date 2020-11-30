//
// Created by dmitry on 3/29/20.
//

#ifndef ANTIPATTERNS_BUTTON_H
#define ANTIPATTERNS_BUTTON_H

#include "GuiFunctions.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_map>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "../Json.h"

namespace gui {

enum class ButtonState { IDLE, HOVER, ACTIVE };
struct ButtonColors {
  sf::Color idle;
  sf::Color hover;
  sf::Color active;
};
struct TextColors {
  sf::Color idle;
  sf::Color hover;
  sf::Color active;
};
struct ButtonParams {
  float x;
  float y;
  float width;
  float height;
  sf::Font *font;
  std::string button_text;
  unsigned int font_size;
  ButtonColors btn_colors;
  TextColors font_colors;
};

class Button {
public:
  Button(const ButtonParams &params);

  virtual ~Button();

  bool IsActive() const;

  void Update(const sf::Vector2f &mouse_pos);

  void Render(sf::RenderTarget &target) const;

private:
  sf::RectangleShape shape_;
  sf::Font *button_font_;
  sf::Text text_;
  ButtonColors btn_colors_;
  TextColors txt_colors_;
  ButtonState button_state_;
};
} // namespace gui

#endif // ANTIPATTERNS_BUTTON_H
