//
// Created by dmitry on 3/28/20.
//

#ifndef ANTIPATTERNS_STATE_H
#define ANTIPATTERNS_STATE_H

#include "../Resources/PauseMenu.h"
#include "../src/Player.h"

struct MousePositions {
  sf::Vector2i screen;
  sf::Vector2i window;
  sf::Vector2f view;
};

class State {
public:
  State(std::shared_ptr<sf::RenderWindow> window,
        std::shared_ptr<std::unordered_map<std::string, int>> supported_keys,
        std::shared_ptr<std::stack<std::shared_ptr<State>>> state_stack);

  virtual ~State();

  /* tools for quitting */
  bool GetToQuit() const;

  /* State functions */
  void UpdateMousePositions();
  /* for pause and quit */
  void Pause();
  void Unpause();
  void End();

  virtual void Render(std::shared_ptr<sf::RenderTarget> target) = 0;
  virtual void Update(float time_elapsed) = 0;
  virtual void UpdateInput(float time_elapsed) = 0;

protected:
  /* declarations */

  /* initializers */
  virtual void InitKeybindings();

  /* variables */
  std::shared_ptr<std::unordered_map<std::string, int>> supported_keys_;
  std::shared_ptr<std::unordered_map<std::string, int>> keybindings_;

  std::shared_ptr<sf::RenderWindow> window_;
  std::shared_ptr<std::stack<std::shared_ptr<State>>> state_stack_;

  bool _to_quit = false;
  bool _paused = false;
  MousePositions mouse_positions_;

private:
  /* still nothing */
};

#endif // ANTIPATTERNS_STATE_H
