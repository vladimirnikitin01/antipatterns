//
// Created by dmitry on 4/1/20.
//

#include "HitboxComponent.h"
#include "iostream"

HitboxComponent::HitboxComponent(
    sf::Sprite &sprite, const std::map<std::string, Json::Node> &settings)
    : sprite_(&sprite) {
  LoadFromMap(settings);
  hitbox_.setPosition(sprite_->getPosition().x + offset_.x,
                      sprite_->getPosition().y + offset_.y);
  hitbox_.setFillColor(sf::Color::Transparent);
  hitbox_.setOutlineColor(sf::Color::Blue);
  hitbox_.setOutlineThickness(1);
}

HitboxComponent::~HitboxComponent() {}

void HitboxComponent::Update() {
  hitbox_.setPosition(sprite_->getPosition().x + offset_.x,
                      sprite_->getPosition().y + offset_.y);
}

void HitboxComponent::Render(sf::RenderTarget &target) const {
  target.draw(hitbox_);
}

void HitboxComponent::SetPosition(const sf::Vector2f &position) {
  hitbox_.setPosition(position);
  sprite_->setPosition(position.x - offset_.x, position.y - offset_.y);
}

void HitboxComponent::UpdateCopy(sf::Sprite &sprite) { sprite_ = &sprite; }

void HitboxComponent::LoadFromMap(
    const std::map<std::string, Json::Node> &settings) {
  float width = static_cast<float>(settings.at("width").AsDouble());
  float height = static_cast<float>(settings.at("height").AsDouble());
  float offset_x = static_cast<float>(settings.at("offset_x").AsDouble());
  float offset_y = static_cast<float>(settings.at("offset_y").AsDouble());
  hitbox_.setSize(sf::Vector2f(width, height));
  offset_ = sf::Vector2f(offset_x, offset_y);
}

sf::RectangleShape HitboxComponent::GetHitbox() const { return hitbox_; }

sf::Vector2f HitboxComponent::GetPosition() const {
  return hitbox_.getPosition();
}

sf::Vector2f HitboxComponent::GetCenteredPosition() const {
  return {hitbox_.getPosition().x + hitbox_.getSize().x / 2.f,
          hitbox_.getPosition().y + hitbox_.getSize().y / 2.f};
}

bool HitboxComponent::Intersects(const sf::RectangleShape &other_hitbox) const {
  return hitbox_.getGlobalBounds().intersects(other_hitbox.getGlobalBounds());
}

bool HitboxComponent::Contains(const sf::RectangleShape &other_hitbox) const {
  sf::FloatRect hitbox_rect =
      sf::FloatRect(hitbox_.getPosition(), hitbox_.getSize());
  sf::Vector2f position = hitbox_.getPosition();
  for (size_t point_index = 0; point_index < 4; ++point_index) {
    sf::Vector2f curr_point = other_hitbox.getPoint(point_index);
    curr_point.x += position.x, curr_point.y += position.y;
    if (!hitbox_rect.contains(curr_point)) {
      return false;
    }
  }
  return true;
}

bool HitboxComponent::ContainedIn(
    const sf::RectangleShape &other_hitbox) const {
  sf::FloatRect other_rect = other_hitbox.getGlobalBounds();
  sf::RectangleShape hitbox = hitbox_;
  sf::Vector2f position = hitbox.getPosition();
  for (size_t point_index = 0; point_index < 4; ++point_index) {
    sf::Vector2f curr_point = hitbox.getPoint(point_index);
    curr_point.x += position.x, curr_point.y += position.y;
    if (!other_rect.contains(curr_point)) {
      return false;
    }
  }
  return true;
}
