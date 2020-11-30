//
// Created by dmitry on 5/3/20.
//

#ifndef ANTIPATTERNS_COMMAND_H
#define ANTIPATTERNS_COMMAND_H

#include <utility>

#include "../Player.h"
#include "Message.h"

class GameCommand {
public:
  explicit GameCommand(std::shared_ptr<Creature> actor)
      : actor_(std::move(actor)) {}

  virtual void Execute(float time_elapsed) = 0;

  virtual void Undo() = 0;

  virtual std::unique_ptr<message::Message> GetMessage() const = 0;

  bool IsDone();

protected:
  std::shared_ptr<Creature> actor_;
  bool is_done{false};
};

class MoveCommand : public GameCommand {
public:
  MoveCommand(std::shared_ptr<Creature> actor, float time_elapsed,
              sf::Vector2f direction);

  void Execute(float time_elapsed) override;

  void Undo() override;

  std::unique_ptr<message::Message> GetMessage() const override;

private:
  float time_elapsed_;
  sf::Vector2f direction_;
};

class TeleportCommand : public GameCommand {
public:
  explicit TeleportCommand(std::shared_ptr<Creature> actor);

  void Execute(float time_elapsed) override;

  void Undo() override;

  static void Update(float time_elapsed);

  std::unique_ptr<message::Message> GetMessage() const override;

private:
  float time_elapsed_{0};
  static float time_left_;
};

class BaseSkillCommand : public GameCommand {
public:
  BaseSkillCommand(std::shared_ptr<Creature> actor, std::string skill_key,
                   std::string animation_key = "");

  [[nodiscard]] std::unique_ptr<message::Message> GetMessage() const override;

  void Execute(float time_elapsed) override;

  void Undo() override;

private:
  std::string skill_key_;
  std::string animation_key_;
};

#endif // ANTIPATTERNS_COMMAND_H
