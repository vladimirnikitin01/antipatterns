//
// Created by dmitry on 3/28/20.
//

#include "Creature.h"

Creature::Creature(const sf::Texture& texture_sheet, const std::map<std::string, Json::Node>& settings) {
  InitPhysicsComponent(settings.at("physics_component").AsMap());
  InitGraphicsComponent(texture_sheet,
                        settings.at("graphics_component").AsMap());
  InitHitboxComponent(settings.at("hitbox_component").AsMap());
  InitAttributeComponent(settings.at("attribute_component").AsMap());
  InitSkillComponent(settings.at("skill_component").AsMap());
  InitExpComponent(1);
}

Creature::~Creature() {}

Creature &Creature::operator=(const Creature &other) {
  if (&other == this) {
    return *this;
  }
  sprite_ = other.sprite_;
  phys_comp_ = std::make_unique<PhysicsComponent>(*other.phys_comp_);
  graph_comp_ = std::make_unique<GraphicsComponent>(*other.graph_comp_);
  attribute_comp_ =
      std::make_unique<AttributeComponent>(*other.attribute_comp_);
  hitbox_comp_ = std::make_unique<HitboxComponent>(*other.hitbox_comp_);
  exp_comp_ = std::make_unique<ExpComponent>(*other.exp_comp_);
  skill_comp_ = std::make_unique<SkillComponent>(*other.skill_comp_);
  phys_comp_->UpdateCopy(sprite_);
  graph_comp_->UpdateCopy(sprite_);
  hitbox_comp_->UpdateCopy(sprite_);
  return *this;
}

Creature::Creature(const Creature &other) { *this = other; }

void Creature::SetTexture(sf::Texture &texture) { sprite_.setTexture(texture); }

void Creature::SetPosition(const sf::Vector2f &position) {
  if (hitbox_comp_ != nullptr) {
    hitbox_comp_->SetPosition(position);
  } else {
    sprite_.setPosition(position);
  }
}

void Creature::SetCenteredPosition(const sf::Vector2f &position) {
  sf::Vector2f size = hitbox_comp_->GetHitbox().getSize();
  SetPosition({position.x - size.x / 2, position.y - size.y / 2});
}

void Creature::Move(const float time_elapsed, const sf::Vector2f &direction) {
  if (this->phys_comp_ != nullptr) {
    phys_comp_->Accelerate(time_elapsed, direction);
  }
}

void Creature::InitPhysicsComponent(
    const std::map<std::string, Json::Node> &settings) {
  phys_comp_ = std::make_unique<PhysicsComponent>(sprite_, settings);
}

void Creature::InitGraphicsComponent(
    const sf::Texture &texture_sheet,
    const std::map<std::string, Json::Node> &settings) {
  graph_comp_ =
      std::make_unique<GraphicsComponent>(sprite_, texture_sheet, settings);
}

void Creature::InitHitboxComponent(
    const std::map<std::string, Json::Node> &settings) {
  hitbox_comp_ = std::make_unique<HitboxComponent>(sprite_, settings);
}

void Creature::InitAttributeComponent(
    const std::map<std::string, Json::Node> &settings) {
  attribute_comp_ = std::make_unique<AttributeComponent>(settings);
}

void Creature::InitExpComponent(int level) {
  exp_comp_ = std::make_unique<ExpComponent>(level);
}


void Creature::InitSkillComponent(
    const std::map<std::string, Json::Node> &settings) {
  skill_comp_ = std::make_unique<SkillComponent>(settings);
}

sf::Vector2f Creature::GetPosition() const {
  return hitbox_comp_ != nullptr ? hitbox_comp_->GetPosition()
                                 : sprite_.getPosition();
}

sf::Vector2f Creature::GetCenteredPosition() const {
  return hitbox_comp_->GetCenteredPosition();
}

sf::RectangleShape Creature::GetHitbox() const {
  if (hitbox_comp_ != nullptr) {
    return hitbox_comp_->GetHitbox();
  }
  sf::FloatRect bounds = sprite_.getGlobalBounds();
  auto result = sf::RectangleShape(sf::Vector2f(bounds.width, bounds.height));
  result.setPosition(sprite_.getPosition());
  return result;
}

void Creature::UpdateMoveAnimations(float time_elapsed) {
  MovementState state = phys_comp_->GetMovementState();
  if (state == MovementState::IDLE) {
    MovementState last_direction = phys_comp_->GetLastMoveDirection();
    if (last_direction == MovementState::IDLE ||
        last_direction == MovementState::MOVING_DOWN) {
      graph_comp_->Play("IDLE_DOWN", time_elapsed);
    } else if (last_direction == MovementState::MOVING_UP) {
      graph_comp_->Play("IDLE_UP", time_elapsed);
    } else if (last_direction == MovementState::MOVING_LEFT) {
      graph_comp_->Play("IDLE_LEFT", time_elapsed);
    } else if (last_direction == MovementState::MOVING_RIGHT) {
      graph_comp_->Play("IDLE_RIGHT", time_elapsed);
    }
  } else {
    if (state == MovementState::MOVING_DOWN) {
      graph_comp_->Play("MOVING_DOWN", time_elapsed,
                        std::abs(phys_comp_->GetLastVelocity().y) /
                            phys_comp_->GetMaxVelocity());
    } else if (state == MovementState::MOVING_UP) {
      graph_comp_->Play("MOVING_UP", time_elapsed,
                        std::abs(phys_comp_->GetLastVelocity().y) /
                            phys_comp_->GetMaxVelocity());
    } else if (state == MovementState::MOVING_LEFT) {
      graph_comp_->Play("MOVING_LEFT", time_elapsed,
                        std::abs(phys_comp_->GetLastVelocity().x) /
                            phys_comp_->GetMaxVelocity());
    } else if (state == MovementState::MOVING_RIGHT) {
      graph_comp_->Play("MOVING_RIGHT", time_elapsed,
                        std::abs(phys_comp_->GetLastVelocity().x) /
                            phys_comp_->GetMaxVelocity());
    }
  }
}

float Creature::GetDistance(const Creature &other) const {
  sf::Vector2f pos = GetCenteredPosition(),
               other_pos = other.GetCenteredPosition();
  return utility::GetDistance(pos, other_pos);
}

const std::unique_ptr<PhysicsComponent> &Creature::GetPhysicsComponent() const {
  return phys_comp_;
}

const std::unique_ptr<GraphicsComponent> &
Creature::GetGraphicsComponent() const {
  return graph_comp_;
}

const std::unique_ptr<HitboxComponent> &Creature::GetHitboxComponent() const {
  return hitbox_comp_;
}

const std::unique_ptr<AttributeComponent> &
Creature::GetAttributeComponent() const {
  return attribute_comp_;
}
const std::unique_ptr<ExpComponent> &Creature::GetExpComponent() const {
  return exp_comp_;
}
const std::unique_ptr<SkillComponent> &Creature::GetSkillComponent() const {
  return skill_comp_;
}
bool Creature::IsDead() const {
  return attribute_comp_->GetAttributeValue(ATTRIBUTE_ID::CURR_HP) <= 0;
}
