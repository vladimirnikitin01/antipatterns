//
// Created by dmitry on 4/2/20.
//

#include "EnemySpawner.h"

EnemySpawner::EnemySpawner(const sf::Texture &texture_sheet,
                           const std::map<std::string, Json::Node> &settings,
                           std::shared_ptr<Enemy> prototype)
    : Enemy(texture_sheet, settings.at("enemy_settings").AsMap()),
      prototype_(std::move(prototype)) {
  InitStates(settings.at("states").AsMap());
}

EnemySpawner::EnemySpawner(const EnemySpawner &other) : Enemy(other) {
  spawn_clock_.restart();
}

void EnemySpawner::Update(float time_elapsed) {
  phys_comp_->Update(time_elapsed);
  attribute_comp_->Update(time_elapsed);
  UpdateAnimations(time_elapsed);
  hitbox_comp_->Update();
  skill_comp_->Update(time_elapsed);
  if (spawn_clock_.getElapsedTime().asSeconds() >= room_const::kSpawnTime) {
    CreateEnemy(exp_comp_->GetLevel());
    spawn_clock_.restart();
  }
}

void EnemySpawner::UpdateEnemy(float time_elapsed,
                               std::shared_ptr<Creature> &player) {
  Enemy::UpdateEnemy(time_elapsed, player);
}

void EnemySpawner::Render(sf::RenderTarget &target) const {
  target.draw(sprite_);
  hitbox_comp_->Render(target);
}

std::unique_ptr<Enemy> EnemySpawner::Clone() const {
  return std::make_unique<EnemySpawner>(*this);
}

void EnemySpawner::CreateEnemy(int level) {
  std::unique_ptr<Enemy> clone = prototype_->Clone();
  sf::Vector2f random_direction = utility::GenerateRandomDirection();
  clone->SetPosition(sf::Vector2f(
      GetCenteredPosition().x + room_const::kSpawnDistance * random_direction.x,
      GetCenteredPosition().y +
          room_const::kSpawnDistance * random_direction.y));
  clone->GenerateAttributes(level);
  clone->SetPursuingStrategy(a_star_);
  spawned_enemies_->push_back(std::move(clone));
}

void EnemySpawner::SetPrototype(std::shared_ptr<Enemy> prototype) {
  prototype_ = std::move(prototype);
}

void EnemySpawner::UpdateAnimations(float time_elapsed) {
  Creature::UpdateMoveAnimations(time_elapsed);
}
void EnemySpawner::SetListToSpawn(std::list<std::unique_ptr<Enemy>> &enemy) {
  spawned_enemies_ = &enemy;
}
