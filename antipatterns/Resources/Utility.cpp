//
// Created by dmitry on 5/1/20.
//

#include "Utility.h"

namespace utility {

float GetDistance(sf::Vector2f lhs, sf::Vector2f rhs) {
  float delta_x = rhs.x - lhs.x, delta_y = rhs.y - lhs.y;
  return sqrtf(delta_x * delta_x + delta_y * delta_y);
}
sf::FloatRect CreateSideRect(sf::Vector2f start, sf::Vector2f end) {
  float width = move_const::kHitboxRecSide * (end.x - start.x);
  float height = move_const::kHitboxRecSide * (end.y - start.y);
  float x =
      start.x + (1.0f - move_const::kHitboxRecSide) / 2 * (end.x - start.x);
  float y =
      start.y + (1.0f - move_const::kHitboxRecSide) / 2 * (end.y - start.y);
  return {{x, y}, {width, height}};
}

sf::Vector2f GetDirection(MovementState state) {
  if (state == MovementState::MOVING_LEFT) {
    return {-1.0f, 0.f};
  } else if (state == MovementState::MOVING_RIGHT) {
    return {1.0f, 0.f};
  } else if (state == MovementState::MOVING_UP) {
    return {0.f, -1.f};
  }
  return {0.f, 1.f};
}

float ScalarProduct(sf::Vector2f lhs, sf::Vector2f rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y;
}

float ScalarProduct(sf::Vector2f lhs, sf::Vector2f rhs_start,
                             sf::Vector2f rhs_end) {
  return ScalarProduct(lhs, {rhs_end.x - rhs_start.x, rhs_end.y - rhs_start.y});
}
float GetSpecialDistance(sf::Vector2f from, const sf::RectangleShape &shape) {
  float half_size_x = shape.getSize().x / 2,
      half_size_y = shape.getSize().y / 2;
  sf::Vector2f center_pos = shape.getPosition();
  center_pos.x += half_size_x;
  center_pos.y += half_size_y;
  float min_dist =
      GetDistance(from, {center_pos.x - half_size_x, center_pos.y});
  min_dist = std::min(
      min_dist, GetDistance(from, {center_pos.x + half_size_x, center_pos.y}));
  min_dist = std::min(
      min_dist, GetDistance(from, {center_pos.x, center_pos.y - half_size_y}));
  min_dist = std::min(
      min_dist, GetDistance(from, {center_pos.x, center_pos.y + half_size_y}));
  return min_dist;
}
sf::Vector2f GenerateRandomDirection() {
  double cos_a = RandomDevice::Instance().RandomNumber(0.0, 1.0);
  double sin_a = sqrt(1.0 - cos_a * cos_a);
  double rand = RandomDevice::Instance().RandomNumber(0, 1);
  sin_a = (rand == 0) ? sin_a : -sin_a;
  return {static_cast<float>(cos_a), static_cast<float>(sin_a)};
}
std::string GetCapsDirection(MovementState state) {
  if (state == MovementState::MOVING_UP) {
    return "UP";
  } else if (state == MovementState::MOVING_LEFT) {
    return "LEFT";
  } else if (state == MovementState::MOVING_RIGHT) {
    return "RIGHT";
  }
  return "DOWN";
}

} // namespace utility