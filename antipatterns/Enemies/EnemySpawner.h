//
// Created by dmitry on 4/2/20.
//

#ifndef ANTIPATTERNS_ENEMYSPAWNER_H
#define ANTIPATTERNS_ENEMYSPAWNER_H

#include "../src/Enemy.h"

class EnemySpawner : public Enemy {
public:
  EnemySpawner(const sf::Texture &texture_sheet,
               const std::map<std::string, Json::Node> &settings,
               std::shared_ptr<Enemy> prototype = nullptr);

  EnemySpawner(const EnemySpawner &other);

  ~EnemySpawner() = default;

  void Update(float time_elapsed) override;
  void UpdateEnemy(float time_elapsed,
                   std::shared_ptr<Creature> &player) override;

  void SetListToSpawn(std::list<std::unique_ptr<Enemy>>& enemy);

  void Render(sf::RenderTarget &target) const override;
  std::unique_ptr<Enemy> Clone() const override;

  void CreateEnemy(int level);

  void SetPrototype(std::shared_ptr<Enemy> prototype);

private:
  void UpdateAnimations(float time_elapsed);

  /* the enemy to be cloned */
  std::shared_ptr<Enemy> prototype_;
  std::list<std::unique_ptr<Enemy>>* spawned_enemies_ = nullptr;
  sf::Clock spawn_clock_;
};

#endif // ANTIPATTERNS_ENEMYSPAWNER_H
