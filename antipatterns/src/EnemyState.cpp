//
// Created by dmitry on 5/4/20.
//

#include "EnemyState.h"
#include "../Enemies/Enemies.h"
#include "Enemy.h"

EnemyStateType EnemyState::GetStateType() const { return type_; }
EnemyState::EnemyState(EnemyStateType type) : type_(type) {}

IdleState::IdleState() : EnemyState(EnemyStateType::IDLE) {}

EnemyStateType IdleState::Update(float time_elapsed, Enemy &enemy,
                                 std::shared_ptr<Creature> &player) {
  if (enemy.GetDistance(*player) > enemy.GetAggroRadius()) {
    return EnemyStateType::IDLE;
  } else {
    sf::Vector2f enemy_pos = enemy.GetCenteredPosition();
    sf::Vector2f pl_pos = player->GetCenteredPosition();
    if (enemy.GetPursuingStrategy()->IsLineSolid(enemy_pos.x, enemy_pos.y,
                                                 pl_pos.x, pl_pos.y)) {
      return EnemyStateType::IDLE;
    }
    return EnemyStateType::PURSUING;
  }
}

PursuingState::PursuingState() : EnemyState(EnemyStateType::PURSUING) {}

EnemyStateType PursuingState::Update(float time_elapsed, Enemy &enemy,
                                     std::shared_ptr<Creature> &player) {

  if (enemy.GetDistance(*player) >= enemy.GetAggroRadius()) {
    last_point_ = nullptr;
    return EnemyStateType::IDLE;
  }
  if (enemy.GetDistance(*player) <= enemy.GetAttackRadius()) {
    last_point_ = nullptr;
    return EnemyStateType::FIGHTING;
  }
  const std::shared_ptr<AStar> &algo = enemy.GetPursuingStrategy();
  int size = algo->GetGridSize();
  std::function<int(float)> div_and_round = [size](float x) {
    return static_cast<int>(std::floor(x)) / size;
  };
  sf::Vector2f enemy_pos = enemy.GetCenteredPosition();
  sf::Vector2f player_pos = player->GetCenteredPosition();
  std::pair<int, int> enemy_pos_grid{div_and_round(enemy_pos.x),
                                     div_and_round(enemy_pos.y)};
  std::pair<int, int> player_pos_grid{div_and_round(player_pos.x),
                                      div_and_round(player_pos.y)};
  //если алгоритм поиска ни разу не запускался
  if (last_point_ == nullptr) {
    last_point_ = std::make_unique<std::pair<int, int>>(
        algo->GetPoint(enemy_pos_grid, player_pos_grid));
  }
  //если можно идти к игроку (напрямую, т.к он в следующей или текущей клетке)
  if (player_pos_grid == *last_point_ || player_pos_grid == enemy_pos_grid) {
    enemy.Move(time_elapsed,
               {player_pos.x - enemy_pos.x, player_pos.y - enemy_pos.y});
    last_point_ = nullptr;
    return EnemyStateType::PURSUING;
  }
  //если мы не дошли до предыдущего запроса алгоритма поиска
  sf::Vector2f desired_pos = {(static_cast<float>(last_point_->first) + 0.5f) *
                                  static_cast<float>(size),
                              (static_cast<float>(last_point_->second) + 0.5f) *
                                  static_cast<float>(size)};
  if (utility::GetDistance(enemy_pos, desired_pos) >
      move_const::kSmallDistance) {
    enemy.Move(time_elapsed,
               {desired_pos.x - enemy_pos.x, desired_pos.y - enemy_pos.y});
    return EnemyStateType::PURSUING;
  }
  //иначе - если мы уже дошли до нужной точки и игрок не рядом - считаем
  //следующую
  std::pair<int, int> next_point =
      algo->GetPoint(enemy_pos_grid, player_pos_grid);

  *last_point_ = next_point;
  desired_pos = {(static_cast<float>(last_point_->first) + 0.5f) *
                     static_cast<float>(size),
                 (static_cast<float>(last_point_->second) + 0.5f) *
                     static_cast<float>(size)};
  if (utility::GetDistance(enemy_pos, desired_pos) >
      move_const::kSmallDistance) {
    enemy.Move(time_elapsed,
               {desired_pos.x - enemy_pos.x, desired_pos.y - enemy_pos.y});
  }
  return EnemyStateType::PURSUING;
}
FightingState::FightingState() : EnemyState(EnemyStateType::FIGHTING) {}

EnemyStateType FightingState::Update(float time_elapsed, Enemy &enemy,
                                     std::shared_ptr<Creature> &player) {
  float distance = enemy.GetDistance(*player);
  Skill *skill = enemy.GetSkillComponent()->GetBestSkill(distance);
  if (skill == nullptr) {
    if (distance <= enemy.GetAttackRadius()) {
      return EnemyStateType::FIGHTING;
    } else if (distance >= enemy.GetAggroRadius()) {
      return EnemyStateType::IDLE;
    }
    return EnemyStateType::PURSUING;
  }
  //если есть способность
  if (skill->GetAllData().range >= enemy.GetDistance(*player)) {
    skill->Cast();
    if (skill->IsCasted()) {
      skill->UpdateAttributes(enemy.GetAttributeComponent().get(),
                              player->GetAttributeComponent().get());
      skill->Reset();
    }
    return EnemyStateType::FIGHTING;
  }
  return EnemyStateType::PURSUING;
}
