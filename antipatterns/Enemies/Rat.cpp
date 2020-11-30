//
// Created by dmitry on 4/2/20.
//

#include "Rat.h"

Rat::Rat(const sf::Texture &texture_sheet,
         const std::map<std::string, Json::Node> &settings)
    : Enemy(texture_sheet, settings.at("enemy_settings").AsMap()) {
  InitStates(settings.at("states").AsMap());
}

Rat::Rat(const Rat &other) : Enemy(other) {

}

void Rat::Update(float time_elapsed) {
  phys_comp_->Update(time_elapsed);
  skill_comp_->Update(time_elapsed);
  attribute_comp_->Update(time_elapsed);
  /* here we use GetMovementState from PhysicsComponent and play animations */
  UpdateAnimations(time_elapsed);
  /* here animations end */
  hitbox_comp_->Update();
}

void Rat::Render(sf::RenderTarget &target) const {
  target.draw(sprite_);
  hitbox_comp_->Render(target);
}

void Rat::UpdateAnimations(float time_elapsed) {
  Creature::UpdateMoveAnimations(time_elapsed);
}

std::unique_ptr<Enemy> Rat::Clone() const {
  return std::make_unique<Rat>(*this);
}

