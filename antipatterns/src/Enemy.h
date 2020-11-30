//
// Created by dmitry on 3/31/20.
//

#ifndef ANTIPATTERNS_ENEMY_H
#define ANTIPATTERNS_ENEMY_H

#include "../Enemies/EnemyTypes.h"
#include "Creature.h"
#include "EnemyState.h"

class Enemy : public Creature {
public:
  Enemy(const sf::Texture &texture_sheet,
        const std::map<std::string, Json::Node> &settings);
  Enemy(const Enemy &other);
  ~Enemy() override;

  void SetPursuingStrategy(std::shared_ptr<AStar> a_star);
  std::shared_ptr<AStar> GetPursuingStrategy() const;
  float GetAggroRadius() const;
  float GetAttackRadius() const;
  sf::Vector2f GetBasePoint() const;

  void Revive();

  void SetPosition(const sf::Vector2f &position) override;
  void SetCenteredPosition(const sf::Vector2f &position) override;

  virtual void UpdateEnemy(float time_elapsed,
                           std::shared_ptr<Creature> &player);

  void GenerateAttributes(int level);
  virtual std::unique_ptr<Enemy> Clone() const = 0;


protected:
  void InitStates(const std::map<std::string, Json::Node> &settings);

  sf::Vector2f base_point_;
  float aggro_radius_;
  float attack_radius_;
  std::shared_ptr<AStar> a_star_;
  std::unordered_map<EnemyStateType, std::unique_ptr<EnemyState>> states_;
  EnemyStateType current_state_id_{EnemyStateType::IDLE};
};

#endif // ANTIPATTERNS_ENEMY_H
