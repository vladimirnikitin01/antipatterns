//
// Created by dmitry on 5/4/20.
//

#ifndef ANTIPATTERNS_ENEMYSTATE_H
#define ANTIPATTERNS_ENEMYSTATE_H

#include "../Resources/AStar.h"
#include "Creature.h"
#include "TileMap.h"

class Enemy;

enum class EnemyStateType { IDLE = 0, PURSUING = 1, FIGHTING = 2 };

class EnemyState {
public:
  explicit EnemyState(EnemyStateType type);

  virtual EnemyStateType Update(float time_elapsed, Enemy &enemy,
                                std::shared_ptr<Creature> &player) = 0;

  EnemyStateType GetStateType() const;

private:
  EnemyStateType type_{};
};

class PursuingState : public EnemyState {
public:
  PursuingState();

  EnemyStateType Update(float time_elapsed, Enemy &enemy,
                        std::shared_ptr<Creature> &player) override;

private:
  sf::Vector2f base_point_;
  std::unique_ptr<std::pair<int, int>> last_point_{nullptr};
};

class FightingState : public EnemyState {
public:
  FightingState();

  EnemyStateType Update(float time_elapsed, Enemy &enemy,
                        std::shared_ptr<Creature> &player) override;

private:

};

class IdleState : public EnemyState {
public:
  IdleState();

  EnemyStateType Update(float time_elapsed, Enemy &enemy,
                        std::shared_ptr<Creature> &player) override;

private:

};


class SpawnerFightingState : public EnemyState {
public:
  SpawnerFightingState() = default;

  EnemyStateType Update(float time_elapsed, Enemy &enemy,
                        std::shared_ptr<Creature> &player) override;

private:
};

class SpawnerIdleState : public EnemyState {
public:
  SpawnerIdleState();

  EnemyStateType Update(float time_elapsed, Enemy &enemy,
                        std::shared_ptr<Creature> &player) override;

private:
  float aggro_radius_;
  float attack_radius_;
};

#endif // ANTIPATTERNS_ENEMYSTATE_H
