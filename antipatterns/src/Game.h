//
// Created by dmitry on 3/28/20.
//

#ifndef ANTIPATTERNS_GAME_H
#define ANTIPATTERNS_GAME_H

#include "../States/MainMenuState.h"

class Game {
public:
  Game();
  ~Game();

  void Update();
  void UpdateEvents();
  void UpdateTime();

  void Run();

  void Render();

  void EndApplication();

private:
  void InitWindow();
  void InitKeys();
  void InitStates();

  sf::Event event_{};

  sf::Clock game_clock_;
  float time_elapsed_{};

  GraphicsSettings _graphics_settings;

  std::shared_ptr<sf::RenderWindow> window_;
  std::shared_ptr<std::stack<std::shared_ptr<State>>> states_;
  std::shared_ptr<std::unordered_map<std::string, int>> supported_keys_;
};

#endif // ANTIPATTERNS_GAME_H
