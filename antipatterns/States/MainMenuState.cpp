//
// Created by dmitry on 3/28/20.
//

#include "MainMenuState.h"

/* constructors and destructors */

MainMenuState::MainMenuState(
    std::shared_ptr<sf::RenderWindow> window,
    std::shared_ptr<std::unordered_map<std::string, int>> supported_keys,
    std::shared_ptr<std::stack<std::shared_ptr<State>>> state_stack)
    : State(std::move(window), std::move(supported_keys),
            std::move(state_stack)),
      btn_manager_(file::kMainMenuButtonsFile, file::kMainFontFile) {
  InitBackground();
  MainMenuState::InitKeybindings();
}

MainMenuState::~MainMenuState() {}

/* overrided functions */

void MainMenuState::Update(const float time_elapsed) {
  UpdateMousePositions();
  UpdateInput(time_elapsed);
  btn_manager_.Update(mouse_positions_.view);

  /* updating all the buttons */
  /* quit the game */
  if (btn_manager_[btn::kMainEndGameBtn]->IsActive()) {
    _to_quit = true;
  }

  /* new game */
  /* pushing a state */
  if (btn_manager_[btn::kMainStartGameBtn]->IsActive()) {
    state_stack_->push(
        std::make_shared<GameState>(window_, supported_keys_, state_stack_));
  }
}

void MainMenuState::UpdateInput(const float time_elapsed) {}

void MainMenuState::Render(std::shared_ptr<sf::RenderTarget> target) {
  /* временный костыль */
  if (target == nullptr) {
    target = window_;
  }
  target->draw(background_._image);
  btn_manager_.Render(*target);
}

void MainMenuState::InitKeybindings() {
  std::ifstream in(file::kMainMenuStateKeybindingsFile);
  if (in.is_open()) {
    std::string key_str;
    std::string key_bind;
    while (in >> key_str >> key_bind) {
      keybindings_->try_emplace(key_str, supported_keys_->at(key_bind));
    }
  }
}

void MainMenuState::InitBackground() {
  background_._image.setSize(static_cast<sf::Vector2f>(window_->getSize()));
  if (!background_._back_texture.loadFromFile(file::kMainMenuBackgroundFile)) {
    throw std::runtime_error("cannot load texture for main menu background");
  }
  background_._image.setTexture(&background_._back_texture);
}
