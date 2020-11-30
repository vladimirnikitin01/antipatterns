//
// Created by dmitry on 3/28/20.
//

#include "Game.h"
#include "../States/GameState.h"

Game::Game() {
  InitWindow();
  /* init keys before init states */
  InitKeys();
  /* do not put before init keys */
  InitStates();
}

Game::~Game() {
  while (!states_->empty()) {
    states_->pop();
  }
}

/* Functions */

void Game::Run() {
  while (window_->isOpen()) {
    UpdateTime();
    Update();
    Render();
  }
}

void Game::Update() {
  UpdateEvents();
  if (!states_->empty()) {
    states_->top()->Update(time_elapsed_);
    if (states_->top()->GetToQuit()) {
      states_->pop();
    }
  } else {
    EndApplication();
    window_->close();
  }
}

void Game::UpdateEvents() {
  while (window_->pollEvent(event_)) {
    if (event_.type == sf::Event::Closed) {
      window_->close();
    }
  }
}

void Game::UpdateTime() {
  /* get how much time elapsed while drawing one frame */
  time_elapsed_ = game_clock_.restart().asSeconds();
}

void Game::Render() {
  window_->clear();

  /* draw everything here actually */
  if (!states_->empty()) {
    states_->top()->Render(window_);
  }

  window_->display();
}

void Game::EndApplication() { std::cout << "Application Ending" << std::endl; }

/* initializers */

void Game::InitWindow() {
  if (!_graphics_settings.LoadFromFile(file::kWindowSettingsFile)) {
    throw std::runtime_error("settings not found");
  }

  if (_graphics_settings.fullscreen_) {
    window_ = std::make_shared<sf::RenderWindow>(
        _graphics_settings.resolution_, _graphics_settings.game_title_,
        sf::Style::Fullscreen, _graphics_settings.settings_);
  } else {
    window_ = std::make_shared<sf::RenderWindow>(
        _graphics_settings.resolution_, _graphics_settings.game_title_,
        sf::Style::Titlebar | sf::Style::Close, _graphics_settings.settings_);
  }
  window_->setFramerateLimit(_graphics_settings.framerate_limit_);
  window_->setVerticalSyncEnabled(_graphics_settings.v_sync_enabled_);
}

void Game::InitKeys() {
  supported_keys_ = std::make_shared<std::unordered_map<std::string, int>>();
  std::ifstream in(file::kSupportedKeysFile);
  if (in.is_open()) {
    std::string key_str{};
    int key_val{};
    while (in >> key_str >> key_val) {
      supported_keys_->try_emplace(key_str,
                                   static_cast<sf::Keyboard::Key>(key_val));
    }
  }
}

void Game::InitStates() {
  states_ = std::make_shared<std::stack<std::shared_ptr<State>>>();
  states_->push(
      std::make_shared<MainMenuState>(window_, supported_keys_, states_));
}
