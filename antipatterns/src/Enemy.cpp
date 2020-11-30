//
// Created by dmitry on 3/31/20.
//

#include "Enemy.h"

Enemy::Enemy(const sf::Texture &texture_sheet,
             const std::map<std::string, Json::Node> &settings)
    : Creature(texture_sheet, settings) {}

Enemy::Enemy(const Enemy &other) : Creature(other) {
  aggro_radius_ = other.aggro_radius_;
  attack_radius_ = other.attack_radius_;
  states_[EnemyStateType::IDLE] = std::make_unique<IdleState>();
  states_[EnemyStateType::PURSUING] = std::make_unique<PursuingState>();
  states_[EnemyStateType::FIGHTING] = std::make_unique<FightingState>();
}

Enemy::~Enemy() {}

void Enemy::Revive() {
  current_state_id_ = EnemyStateType::IDLE;
  SetPosition(base_point_);
  attribute_comp_->ResetHpMana();
}

void Enemy::SetPursuingStrategy(std::shared_ptr<AStar> a_star) {
  a_star_ = std::move(a_star);
}

std::shared_ptr<AStar> Enemy::GetPursuingStrategy() const { return a_star_; }

float Enemy::GetAggroRadius() const { return aggro_radius_; }
float Enemy::GetAttackRadius() const { return attack_radius_; }
sf::Vector2f Enemy::GetBasePoint() const { return base_point_; }

void Enemy::GenerateAttributes(int level) {}

void Enemy::UpdateEnemy(float time_elapsed, std::shared_ptr<Creature> &player) {
  EnemyStateType new_id =
      states_.at(current_state_id_)->Update(time_elapsed, *this, player);
  if (new_id != current_state_id_) {
    current_state_id_ = new_id;
  }
}

void Enemy::SetPosition(const sf::Vector2f &position) {
  Creature::SetPosition(position);
  base_point_ = position;
}

void Enemy::SetCenteredPosition(const sf::Vector2f &position) {
  Creature::SetCenteredPosition(position);
  base_point_ = position;
}

void Enemy::InitStates(const std::map<std::string, Json::Node> &settings) {
  aggro_radius_ = settings.at("aggro_radius").AsFloat();
  attack_radius_ = settings.at("attack_radius").AsFloat();
  states_[EnemyStateType::IDLE] = std::make_unique<IdleState>();
  states_[EnemyStateType::PURSUING] = std::make_unique<PursuingState>();
  states_[EnemyStateType::FIGHTING] = std::make_unique<FightingState>();
}
