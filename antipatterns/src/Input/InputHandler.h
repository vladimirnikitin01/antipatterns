//
// Created by dmitry on 5/3/20.
//

#ifndef ANTIPATTERNS_INPUTHANDLER_H
#define ANTIPATTERNS_INPUTHANDLER_H

#include "Command.h"

class PlayerInputHandler {
public:
  PlayerInputHandler(
      std::shared_ptr<Creature> player,
      std::shared_ptr<std::unordered_map<std::string, int>> keybindings);

  void UpdateInput(float time_elapsed);

  std::shared_ptr<GameCommand> GetLastCommand();

  void Reset();

private:
  bool IsKeyPressed(const std::string &key);

  void UpdateMoveCommand(float time_elapsed, const std::string &keybind,
                         sf::Vector2f dir);

  void UpdateTeleport(float time_elapsed);

  void UpdateBaseSkill(float time_elapsed, const std::string &keybind,
                       const std::string &skill_key,
                       const std::string &animation_key = "");

  void UpdateAttack(float time_elapsed, const std::string &keybind,
                    const std::string &skill_key,
                    std::string animation_key = "");

  void UpdateCommandCooldowns(float time_elapsed);

  sf::Keyboard::Key last_key_{kUnknownKey};
  std::shared_ptr<std::unordered_map<std::string, int>> keybindings_;
  std::shared_ptr<Creature> player_;
  std::shared_ptr<GameCommand> last_command_;
};

#endif // ANTIPATTERNS_INPUTHANDLER_H
