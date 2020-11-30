//
// Created by dmitry on 4/2/20.
//

#include "EnemySystem.h"

DeadEnemy::DeadEnemy(Enemy *enemy) : enemy(enemy) {

}
void DeadEnemy::Update(float time_elapsed) {
  time_elapsed_ += time_elapsed;
}
bool DeadEnemy::IsReadyToRevive() const {
  return time_elapsed_ >= room_const::kReviveTime;
}

const std::unordered_map<std::string, EnemyType> EnemySystem::names_to_types_ =
    {{"enemy_spawner", EnemyType::ENEMY_SPAWNER}, {"rat", EnemyType::RAT}};

/* constructors and destructors */

EnemySystem::EnemySystem(
    const std::map<std::string, Json::Node> &enemies_settings,
    const std::unordered_map<EnemyType, std::shared_ptr<Enemy>> &unique_enemies,
    const TileMap &tile_map) {
  InitAStar(tile_map.GetCostMap(), TileMap::GetGridSize());
  LoadEnemies(enemies_settings, unique_enemies);
}

EnemySystem::~EnemySystem() {}

void EnemySystem::LoadEnemies(
    const std::map<std::string, Json::Node> &enemies_settings,
    const std::unordered_map<EnemyType, std::shared_ptr<Enemy>>
        &unique_enemies) {
  std::vector<Json::Node> tile_size_array =
      enemies_settings.at("tile_size").AsArray();
  sf::Vector2i tile_size = sf::Vector2i(tile_size_array.at(0).AsDouble(),
                                        tile_size_array.at(1).AsDouble());
  const std::vector<Json::Node> enemies =
      enemies_settings.at("enemies").AsArray();
  for (const auto &node : enemies) {
    const auto &enemy_settings = node.AsMap();
    EnemyType enemy_type =
        names_to_types_.at(enemy_settings.at("type").AsString());
    int enemy_level = static_cast<int>(enemy_settings.at("level").AsDouble());
    EnemyType type_to_spawn{};
    if (enemy_settings.count("type_to_spawn") != 0) {
      type_to_spawn =
          static_cast<EnemyType>(enemy_settings.at("type_to_spawn").AsDouble());
    }
    auto json_pos = enemy_settings.at("pos").AsArray();
    auto rel_pos =
        sf::Vector2f(json_pos.at(0).AsDouble(), json_pos.at(1).AsDouble());
    CreateEnemy({enemy_type, enemy_level, type_to_spawn},
                sf::Vector2f(rel_pos.x * tile_size.x, rel_pos.y * tile_size.y),
                unique_enemies);
  }
}

void EnemySystem::CreateEnemy(
    EnemyParams params, const sf::Vector2f &pos,
    const std::unordered_map<EnemyType, std::shared_ptr<Enemy>>
        &unique_enemies) {
  std::unique_ptr<Enemy> clone = unique_enemies.at(params._type)->Clone();
  clone->GenerateAttributes(params._level);
  /* if is a correct-defined spawner */
  if (params._type_to_spawn != EnemyType::DEFAULT) {
    auto &_spawner = dynamic_cast<EnemySpawner &>(*clone);
    _spawner.SetPrototype(unique_enemies.at(params._type_to_spawn));
    _spawner.SetListToSpawn(active_enemies_);
  }
  clone->SetPosition(pos);
  clone->SetPursuingStrategy(a_star_);
  active_enemies_.push_back(std::move(clone));
}

void EnemySystem::Update(float time_elapsed) {
  if (player_ != nullptr) {
    UpdatePlayer(time_elapsed);
    auto active_it = active_enemies_.begin();
    while (active_it != active_enemies_.end()) {
      (*active_it)->Update(time_elapsed);
      if ((*active_it)->IsDead()) {
        Enemy *enemy = (*active_it).release();
        dead_enemies_.emplace_back(enemy);
        active_it = active_enemies_.erase(active_it);
        continue;
      }
      ++active_it;
    }
  }
  auto dead_it = dead_enemies_.begin();
  while (dead_it != dead_enemies_.end()) {
    dead_it->Update(time_elapsed);
    if (dead_it->IsReadyToRevive()) {
      Enemy* to_revive = dead_it->enemy.release();
      to_revive->Revive();
      active_enemies_.push_back(std::unique_ptr<Enemy>(to_revive));
      dead_it = dead_enemies_.erase(dead_it);
      continue;
    }
    ++dead_it;
  }
}

void EnemySystem::Render(sf::RenderTarget &target) const {
  for (auto &active_enemy : active_enemies_) {
    active_enemy->Render(target);
  }
}

void EnemySystem::SetPlayer(std::shared_ptr<Creature> player) {
  player_ = std::move(player);
}

void EnemySystem::UpdatePlayer(float time_elapsed) {
  for (auto &enemy_alive : active_enemies_) {
    enemy_alive->UpdateEnemy(time_elapsed, player_);
  }
}
void EnemySystem::InitAStar(const std::vector<std::vector<int>> &cost_map,
                            int grid_size) {
  a_star_ = std::make_shared<AStar>(cost_map, grid_size);
}
std::list<std::unique_ptr<Enemy>>::iterator EnemySystem::begin() {
  return active_enemies_.begin();
}
std::list<std::unique_ptr<Enemy>>::iterator EnemySystem::end() {
  return active_enemies_.end();
}
void EnemySystem::ReceiveAttackMessage(
    std::unique_ptr<message::Message> message) {
  Skill *casted_skill = message->GetData().AsSkillData();
  SkillType type = casted_skill->GetType();
  if (type == SkillType::SELF) {
    casted_skill->UpdateAttributes(player_->GetAttributeComponent().get(),
                                   player_->GetAttributeComponent().get());
  } else {
    float range = casted_skill->GetAllData().range;
    MovementState looking =
        player_->GetPhysicsComponent()->GetLastMoveDirection();
    sf::Vector2f dir = utility::GetDirection(looking);
    for (const auto &enemy : active_enemies_) {
      if (utility::ScalarProduct(dir, player_->GetCenteredPosition(),
                                 enemy->GetCenteredPosition()) > 0 &&
          utility::GetSpecialDistance(player_->GetCenteredPosition(),
                                      enemy->GetHitbox()) <= range) {
        casted_skill->UpdateAttributes(player_->GetAttributeComponent().get(),
                                       enemy->GetAttributeComponent().get());
      }
    }
  }
}
