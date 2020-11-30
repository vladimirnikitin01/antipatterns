//
// Created by dmitry on 5/3/20.
//

#include "InputHandler.h"
PlayerInputHandler::PlayerInputHandler(
    std::shared_ptr<Creature> player,
    std::shared_ptr<std::unordered_map<std::string, int>> keybindings)
    : player_(std::move(player)), keybindings_(std::move(keybindings)) {}

void PlayerInputHandler::UpdateInput(float time_elapsed) {
  if (IsKeyPressed(command_const::kMoveLeftKey)) {
    UpdateMoveCommand(time_elapsed, command_const::kMoveLeftKey, {-1.f, 0.f});
  } else if (IsKeyPressed(command_const::kMoveRightKey)) {
    UpdateMoveCommand(time_elapsed, command_const::kMoveRightKey, {1.f, 0.f});
  } else if (IsKeyPressed(command_const::kMoveDownKey)) {
    UpdateMoveCommand(time_elapsed, command_const::kMoveDownKey, {0.f, 1.f});
  } else if (IsKeyPressed(command_const::kMoveUpKey)) {
    UpdateMoveCommand(time_elapsed, command_const::kMoveUpKey, {0.f, -1.f});
  } else if (IsKeyPressed(command_const::kTeleportKeyBind)) {
    UpdateTeleport(time_elapsed);
  } else if (IsKeyPressed(command_const::kHarakiriKeyBind)) {
    UpdateBaseSkill(time_elapsed, command_const::kHarakiriKeyBind,
                    command_const::kHarakiriKey);
  } else if (IsKeyPressed(command_const::kAttackKeyBind)) {
    UpdateAttack(time_elapsed, command_const::kAttackKeyBind,
                 command_const::kAttackKey, command_const::kAttackAnim);
  } else {
    if (last_command_ != nullptr) {
      last_command_->Undo();
      Reset();
    }
  }
  UpdateCommandCooldowns(time_elapsed);
}

std::shared_ptr<GameCommand> PlayerInputHandler::GetLastCommand() {
  return last_command_;
}
void PlayerInputHandler::Reset() {
  last_key_ = kUnknownKey;
  last_command_ = nullptr;
}
bool PlayerInputHandler::IsKeyPressed(const std::string &key) {
  return sf::Keyboard::isKeyPressed(
      static_cast<sf::Keyboard::Key>(keybindings_->at(key)));
}

void PlayerInputHandler::UpdateMoveCommand(float time_elapsed,
                                           const std::string &keybind,
                                           sf::Vector2f dir) {
  if (last_command_ != nullptr)
    last_command_->Undo();
  last_key_ = static_cast<sf::Keyboard::Key>(keybindings_->at(keybind));
  last_command_ = std::make_shared<MoveCommand>(player_, time_elapsed, dir);
}

void PlayerInputHandler::UpdateTeleport(float time_elapsed) {
  TeleportCommand::Update(time_elapsed);
  if (last_key_ !=
      static_cast<sf::Keyboard::Key>(keybindings_->at("TELEPORT"))) {
    if (last_command_ != nullptr)
      last_command_->Undo();
    last_key_ = static_cast<sf::Keyboard::Key>(keybindings_->at("TELEPORT"));
    last_command_ = std::make_shared<TeleportCommand>(player_);
  }
}

void PlayerInputHandler::UpdateBaseSkill(float time_elapsed,
                                         const std::string &keybind,
                                         const std::string &skill_key,
                                         const std::string &animation_key) {
  if (last_key_ != static_cast<sf::Keyboard::Key>(keybindings_->at(keybind))) {
    if (last_command_ != nullptr) {
      last_command_->Undo();
    }
    last_key_ = static_cast<sf::Keyboard::Key>(keybindings_->at(keybind));
    last_command_ =
        std::make_shared<BaseSkillCommand>(player_, skill_key, animation_key);
  }
}
void PlayerInputHandler::UpdateAttack(float time_elapsed,
                                      const std::string &keybind,
                                      const std::string &skill_key,
                                      std::string animation_key) {
  MovementState state =
      player_->GetPhysicsComponent()->GetLastMoveDirection();
  animation_key += "_" + utility::GetCapsDirection(state);
  UpdateBaseSkill(time_elapsed, keybind, skill_key, animation_key);
}
void PlayerInputHandler::UpdateCommandCooldowns(float time_elapsed) {
  TeleportCommand::Update(time_elapsed);
}
