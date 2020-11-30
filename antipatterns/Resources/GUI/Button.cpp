//
// Created by dmitry on 3/29/20.
//

#include "Button.h"

gui::Button::Button(const ButtonParams &params) {
  shape_.setPosition(params.x, params.y);
  shape_.setSize(sf::Vector2f(params.width, params.height));

  button_font_ = params.font;
  text_.setString(params.button_text);
  text_.setFont(*button_font_);
  text_.setFillColor(sf::Color::White);
  text_.setCharacterSize(params.font_size);
  text_.setPosition(
      shape_.getPosition().x + shape_.getGlobalBounds().width / 2 -
          text_.getGlobalBounds().width / 2,
      shape_.getPosition().y + shape_.getGlobalBounds().height / 2 -
          text_.getGlobalBounds().height / 2);

  btn_colors_.active = params.btn_colors.active;
  btn_colors_.idle = params.btn_colors.idle;
  btn_colors_.hover = params.btn_colors.hover;
  txt_colors_.active = params.font_colors.active;
  txt_colors_.idle = params.font_colors.idle;
  txt_colors_.hover = params.font_colors.hover;
}

gui::Button::~Button() {}

void gui::Button::Update(const sf::Vector2f &mouse_pos) {
  button_state_ = gui::ButtonState::IDLE;

  if (shape_.getGlobalBounds().contains(mouse_pos)) {
    button_state_ = gui::ButtonState::HOVER;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      button_state_ = gui::ButtonState::ACTIVE;
    }
  }

  switch (button_state_) {
  case gui::ButtonState::IDLE:
    shape_.setFillColor(btn_colors_.idle);
    text_.setFillColor(txt_colors_.idle);
    break;
  case gui::ButtonState::HOVER:
    shape_.setFillColor(btn_colors_.hover);
    text_.setFillColor(txt_colors_.hover);
    break;
  case gui::ButtonState::ACTIVE:
    shape_.setFillColor(btn_colors_.active);
    text_.setFillColor(btn_colors_.active);
    break;
  default:
    throw(std::runtime_error("No state for button"));
  }
}

void gui::Button::Render(sf::RenderTarget &target) const {
  target.draw(shape_);
  target.draw(text_);
}

bool gui::Button::IsActive() const {
  return button_state_ == gui::ButtonState::ACTIVE;
}
