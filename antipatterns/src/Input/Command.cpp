//
// Created by dmitry on 5/3/20.
//

#include "Command.h"
bool GameCommand::IsDone() { return is_done; }
MoveCommand::MoveCommand(std::shared_ptr<Creature> actor, float time_elapsed,
                         sf::Vector2f direction)
    : GameCommand(std::move(actor)), direction_(direction),
      time_elapsed_(time_elapsed) {}

void MoveCommand::Execute(float time_elapsed) {
  actor_->Move(time_elapsed, direction_);
  is_done = true;
}
void MoveCommand::Undo() {}
std::unique_ptr<message::Message> MoveCommand::GetMessage() const {
  return nullptr;
}

float TeleportCommand::time_left_ = 0;

TeleportCommand::TeleportCommand(std::shared_ptr<Creature> actor)
    : GameCommand(std::move(actor)) {}

void TeleportCommand::Execute(float time_elapsed) {
  if (time_left_ > 0) {
    return;
  }
  time_elapsed_ += time_elapsed;
  if (time_elapsed_ >= command_const::kTeleportTime) {
    MovementState last = actor_->GetPhysicsComponent()->GetLastMoveDirection();
    sf::Vector2f pos = actor_->GetPosition();
    if (last == MovementState::MOVING_LEFT) {
      actor_->SetPosition({pos.x - command_const::kTeleportDistance, pos.y});
    } else if (last == MovementState::MOVING_RIGHT) {
      actor_->SetPosition({pos.x + command_const::kTeleportDistance, pos.y});
    } else if (last == MovementState::MOVING_UP) {
      actor_->SetPosition({pos.x, pos.y - command_const::kTeleportDistance});
    } else if (last == MovementState::MOVING_DOWN) {
      actor_->SetPosition({pos.x, pos.y + command_const::kTeleportDistance});
    }
    is_done = true;
    time_left_ = command_const::kTeleportCoolDown;
  }
}

void TeleportCommand::Undo() { time_elapsed_ = 0; }
void TeleportCommand::Update(float time_elapsed) {
  if (time_left_ > 0) {
    time_left_ -= time_elapsed;
  } else if (time_left_ < 0) {
    time_left_ = 0;
  }
}
std::unique_ptr<message::Message> TeleportCommand::GetMessage() const {
  return nullptr;
}

BaseSkillCommand::BaseSkillCommand(std::shared_ptr<Creature> actor,
                                   std::string skill_key,
                                   std::string animation_key)
    : GameCommand(std::move(actor)), skill_key_(std::move(skill_key)),
      animation_key_(std::move(animation_key)) {}

std::unique_ptr<message::Message> BaseSkillCommand::GetMessage() const {
  return std::make_unique<message::Message>(
      message::Node(actor_->GetSkillComponent()->GetSkill(skill_key_)));
}
void BaseSkillCommand::Execute(float time_elapsed) {
  Skill *skill = actor_->GetSkillComponent()->GetSkill(skill_key_);
  if (!skill->IsOnCooldown()) {
    skill->Cast();
    //если есть анимация
    if (!animation_key_.empty()) {
      actor_->GetGraphicsComponent()->Play(animation_key_, time_elapsed, 1,
                                           true);
    }
    if (skill->IsCasted()) {
      is_done = true;
      skill->Reset();
      if (!animation_key_.empty()) {
        actor_->GetGraphicsComponent()->Reset();
      }
    }
  }
}

void BaseSkillCommand::Undo() {
  actor_->GetSkillComponent()->GetSkill(skill_key_)->Reset();
  if (!animation_key_.empty()) {
    actor_->GetGraphicsComponent()->Reset();
  }
}
