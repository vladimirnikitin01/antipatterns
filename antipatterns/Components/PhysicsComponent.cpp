//
// Created by dmitry on 3/31/20.
//

#include "PhysicsComponent.h"

PossibleDirections::PossibleDirections(bool left, bool right, bool up,
                                       bool down)
    : left(left), right(right), up(up), down(down) {}

PossibleDirections
PossibleDirections::operator&&(const PossibleDirections &other) {
  return {left && other.left, right && other.right, up && other.up,
          down && other.down};
}

PhysicsComponent::PhysicsComponent(
    sf::Sprite &sprite, const std::map<std::string, Json::Node> &settings)
    : sprite_(&sprite) {
  LoadFromMap(settings);
}

void PhysicsComponent::LoadFromMap(
    const std::map<std::string, Json::Node> &settings) {
  max_velocity_ = static_cast<float>(settings.at("max_velocity").AsDouble());
}

PhysicsComponent::~PhysicsComponent() {}

/* getters */
float PhysicsComponent::GetMaxVelocity() const { return max_velocity_; }

sf::Vector2f PhysicsComponent::GetLastVelocity() const {
  return last_velocity_ * speed_multiplier_;
}

MovementState PhysicsComponent::GetMovementState() const {
  if (last_velocity_.x == 0 && last_velocity_.y == 0) {
    return MovementState::IDLE;
  }
  if (last_velocity_.x < 0 && std::abs(last_velocity_.x) >= std::abs(last_velocity_.y)) {
    return MovementState::MOVING_LEFT;
  }
  if (last_velocity_.x > 0 && std::abs(last_velocity_.x) >= std::abs(last_velocity_.y)) {
    return MovementState::MOVING_RIGHT;
  }
  if (last_velocity_.y < 0 && std::abs(last_velocity_.x) < std::abs(last_velocity_.y)) {
    return MovementState::MOVING_UP;
  }
  if (last_velocity_.y > 0 && std::abs(last_velocity_.x) < std::abs(last_velocity_.y)) {
    return MovementState::MOVING_DOWN;
  }
  throw std::runtime_error("CASE NOT HANDLED");
}

MovementState PhysicsComponent::GetLastMoveDirection() const {
  return last_direction_;
}

/* modifiers */
void PhysicsComponent::Stop() { velocity_.x = velocity_.y = 0; }

void PhysicsComponent::SetPossibleMoveDirections(
    PossibleDirections directions) {
  able_dir = directions;
}

void PhysicsComponent::UpdateSpeedMultiplier(float multiplier) {
  speed_multiplier_ = multiplier;
}

void PhysicsComponent::StopAxisMoveX() { velocity_.x = 0; }

void PhysicsComponent::StopAxisMoveY() { velocity_.y = 0; }

/* functions */
void PhysicsComponent::Accelerate(const float time_elapsed,
                                  sf::Vector2f direction) {
  float dir_abs =
      sqrtf(std::abs(direction.x * direction.x + direction.y * direction.y));
  if (dir_abs >= move_const::kSmallValue) {
    direction.x /= dir_abs;
    direction.y /= dir_abs;
  }
  if ((direction.x < 0 && able_dir.left) ||
      (direction.x > 0 && able_dir.right)) {
    velocity_.x = max_velocity_ * direction.x;
  }
  if ((direction.y < 0 && able_dir.up) || (direction.y > 0 && able_dir.down)) {
    velocity_.y = max_velocity_ * direction.y;
  }
}

void PhysicsComponent::Update(const float time_elapsed) {
  sprite_->move(velocity_ * speed_multiplier_ * time_elapsed);
  last_velocity_.x = velocity_.x;
  last_velocity_.y = velocity_.y;
  if (velocity_.x != 0 || velocity_.y != 0) {
    last_direction_ = GetMovementState();
  }
  velocity_.x = 0;
  velocity_.y = 0;
}

void PhysicsComponent::UpdateCopy(sf::Sprite &sprite) { sprite_ = &sprite; }
