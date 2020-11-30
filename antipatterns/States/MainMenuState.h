//
// Created by dmitry on 3/28/20.
//

#ifndef ANTIPATTERNS_MAINMENUSTATE_H
#define ANTIPATTERNS_MAINMENUSTATE_H

#include "../Resources/GUI/ButtonManager.h"
#include "GameState.h"

struct Background {
  sf::RectangleShape _image;
  sf::Texture _back_texture;
};

class MainMenuState : public State {
public:
  MainMenuState(
      std::shared_ptr<sf::RenderWindow> window,
      std::shared_ptr<std::unordered_map<std::string, int>> supported_keys,
      std::shared_ptr<std::stack<std::shared_ptr<State>>> state_stack);
  ~MainMenuState();

  /* overrided functions */
  void Update(float time_elapsed) override;
  void UpdateInput(float time_elapsed) override;
  void Render(std::shared_ptr<sf::RenderTarget> target) override;

private:
  /* initializers */
  void InitKeybindings() override;
  void InitBackground();

  /* variables */
  sf::Font main_font_;
  Background background_;
  gui::ButtonManager btn_manager_;
};

#endif // ANTIPATTERNS_MAINMENUSTATE_H
