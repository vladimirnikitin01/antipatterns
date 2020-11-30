//
// Created by dmitry on 3/30/20.
//

#include "Player.h"

Player::Player(sf::Vector2f position, const sf::Texture &texture_sheet,
               const std::string &file_name) : Creature(texture_sheet, Json::Load(file_name).GetRoot().AsMap()) {
  const std::map<std::string, Json::Node> settings =
      Json::Load(file_name).GetRoot().AsMap();
  Creature::InitSkillComponent(settings.at("skill_component").AsMap());
  Creature::SetPosition(position);
}

Player::~Player() {}

/* overrided functions */

void Player::Update(float time_elapsed) {
  phys_comp_->Update(time_elapsed);
  skill_comp_->Update(time_elapsed);
  UpdateAnimations(time_elapsed);
  hitbox_comp_->Update();
  attribute_comp_->Update(time_elapsed);
}

void Player::Render(sf::RenderTarget &target) const {
  target.draw(sprite_);
  hitbox_comp_->Render(target);
}

/* initializers */

void Player::UpdateAnimations(float time_elapsed) {
  Creature::UpdateMoveAnimations(time_elapsed);
}
