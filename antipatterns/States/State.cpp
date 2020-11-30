//
// Created by dmitry on 3/28/20.
//

#include "State.h"

#include <utility>

/* Constructors and destructors */
State::State(
    std::shared_ptr<sf::RenderWindow> window,
    std::shared_ptr<std::unordered_map<std::string, int>> supported_keys,
    std::shared_ptr<std::stack<std::shared_ptr<State>>> state_stack)
    : window_(std::move(window)), supported_keys_(std::move(supported_keys)),
      state_stack_(std::move(state_stack)) {
  State::InitKeybindings();
}

State::~State() {}

bool State::GetToQuit() const { return _to_quit; }

void State::UpdateMousePositions() {
  mouse_positions_.screen = sf::Mouse::getPosition();
  mouse_positions_.window = sf::Mouse::getPosition(*window_);
  mouse_positions_.view =
      window_->mapPixelToCoords(sf::Mouse::getPosition(*window_));
}

void State::End() { _to_quit = true; }

void State::Pause() { _paused = true; }

void State::Unpause() { _paused = false; }

void State::InitKeybindings() {
  keybindings_ = std::make_shared<std::unordered_map<std::string, int>>();
}
