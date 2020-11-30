//
// Created by dmitry on 4/1/20.
//

#ifndef ANTIPATTERNS_PAUSEMENU_H
#define ANTIPATTERNS_PAUSEMENU_H

#include "GUI/ButtonManager.h"

class PauseMenu {
public:
  explicit PauseMenu(const std::shared_ptr<sf::RenderWindow> &window);
  ~PauseMenu();

  void Update(const sf::Vector2f &mouse_pos);
  void Render(sf::RenderTarget &target) const;

  bool IsButtonActive(const std::string &button_key) const;

private:
  sf::RectangleShape background_;
  sf::RectangleShape inner_;
  sf::Text menu_text_;
  gui::ButtonManager btn_manager_;

  constexpr static const float outer_ratio_x_{1.0};
  constexpr static const float outer_ratio_y_{1.0};

  constexpr static const float inner_ratio_x_{0.4};
  constexpr static const float inner_ratio_y_{1.0};
};

#endif // ANTIPATTERNS_PAUSEMENU_H
