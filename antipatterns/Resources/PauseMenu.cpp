//
// Created by dmitry on 4/1/20.
//

#include "PauseMenu.h"

PauseMenu::PauseMenu(const std::shared_ptr<sf::RenderWindow> &window)
    : btn_manager_(file::kPauseMenuButtonsFile, file::kMainFontFile) {
  inner_.setSize(sf::Vector2f(window->getSize().x * inner_ratio_x_,
                              window->getSize().y * inner_ratio_y_));
  background_.setSize(sf::Vector2f(window->getSize().x * outer_ratio_x_,
                                   window->getSize().y * outer_ratio_y_));
  background_.setFillColor(sf::Color(20, 20, 20, 100));
  inner_.setFillColor(sf::Color(20, 20, 20, 200));

  background_.setPosition(
      window->getSize().x / 2.f - background_.getSize().x / 2.f,
      window->getSize().y / 2.f - background_.getSize().y / 2.f);
  inner_.setPosition(window->getSize().x / 2.f - inner_.getSize().x / 2.f,
                     window->getSize().y / 2.f - inner_.getSize().y / 2.f);
  menu_text_ = sf::Text(btn::kGamePausedText, btn_manager_.GetFont(), 30);
  menu_text_.setPosition(
      window->getSize().x * 0.5 - menu_text_.getGlobalBounds().width * 0.5,
      window->getSize().y * 0.1 - menu_text_.getGlobalBounds().height * 0.5);
  menu_text_.setFillColor(sf::Color(200, 200, 200, 200));
}

PauseMenu::~PauseMenu() {}

/* functions */

void PauseMenu::Update(const sf::Vector2f &mouse_pos) {
  btn_manager_.Update(mouse_pos);
}

void PauseMenu::Render(sf::RenderTarget &target) const {
  target.draw(background_);
  target.draw(inner_);
  target.draw(menu_text_);
  btn_manager_.Render(target);
}

bool PauseMenu::IsButtonActive(const std::string &button_key) const {
  return btn_manager_[button_key]->IsActive();
}
