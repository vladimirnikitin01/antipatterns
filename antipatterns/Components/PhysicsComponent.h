//
// Created by dmitry on 3/31/20.
//

#ifndef ANTIPATTERNS_PHYSICSCOMPONENT_H
#define ANTIPATTERNS_PHYSICSCOMPONENT_H

#include "../Resources/GraphicsSettings.h"
#include "../Resources/Json.h"
#include "../Resources/Constants/GamePlayConstants.h"

enum MovementState {
  IDLE,
  MOVING,
  MOVING_LEFT,
  MOVING_RIGHT,
  MOVING_UP,
  MOVING_DOWN
};

struct PossibleDirections {
  bool left{true};
  bool right{true};
  bool up{true};
  bool down{true};

  PossibleDirections() = default;
  PossibleDirections(bool left, bool right, bool up, bool down);

  PossibleDirections operator&&(const PossibleDirections &other);
};

class PhysicsComponent {
public:
  PhysicsComponent(sf::Sprite &sprite,
                   const std::map<std::string, Json::Node> &settings);
  PhysicsComponent(const PhysicsComponent &other) = default;
  PhysicsComponent &operator=(const PhysicsComponent &other) = default;
  /* initializer */
  void LoadFromMap(const std::map<std::string, Json::Node> &settings);

  ~PhysicsComponent();

  /* getters */
  float GetMaxVelocity() const;
  sf::Vector2f GetLastVelocity() const;
  MovementState GetMovementState() const;
  MovementState GetLastMoveDirection() const;

  /* modifiers */

  void SetPossibleMoveDirections(PossibleDirections directions);
  void UpdateSpeedMultiplier(float multiplier);
  void Stop();
  void StopAxisMoveX();
  void StopAxisMoveY();

  /* functions */
  void Accelerate(float time_elapsed, sf::Vector2f direction);
  void Update(float time_elapsed);
  void UpdateCopy(sf::Sprite &sprite);

private:
  sf::Sprite *sprite_;

  float speed_multiplier_{1};
  PossibleDirections able_dir;
  sf::Vector2f velocity_;
  sf::Vector2f last_velocity_{0, 0};
  MovementState last_direction_{MovementState::IDLE};

  float max_velocity_;
};

#endif // ANTIPATTERNS_PHYSICSCOMPONENT_H
