//
// Created by dmitry on 4/2/20.
//

#ifndef ANTIPATTERNS_TILEMAP_H
#define ANTIPATTERNS_TILEMAP_H

#include "../Resources/Utility.h"
#include "Creature.h"
#include "Tile.h"
class Enemy;

class TileMap {
  using Map = std::vector<std::vector<std::vector<std::unique_ptr<Tile>>>>;

public:
  explicit TileMap(
      const std::string &map_tile_name,
      const std::unordered_map<TileType, std::unique_ptr<Tile>> &unique_tiles);

  ~TileMap() = default;

  template <typename T>

  void UpdateCreature(T &creature, float time_elapsed);

  void Render(sf::RenderTarget &target) const;

  static int GetGridSize();

  static void SetGridSize(int grid_size);

  sf::Vector2i GetGridPosition(sf::Vector2f global_position) const;

  sf::Vector2f GetGlobalPosition(sf::Vector2i grid_position) const;

  std::vector<std::vector<int>> GetCostMap() const;

private:
  void InitMap(
      const std::string &file_name,
      const std::unordered_map<TileType, std::unique_ptr<Tile>> &unique_tiles);
  void AddTileLayer(
      TileType type, int x_pos, int y_pos, sf::Vector2f shift,
      const std::unordered_map<TileType, std::unique_ptr<Tile>> &unique_tiles);
  bool CanMove(const sf::FloatRect &rect);

  template <typename T>
  void CheckBorders(T& creature) {
    if constexpr (std::is_same_v<T, std::shared_ptr<Creature>> ||
                  std::is_same_v<T, std::unique_ptr<Creature>> ||
                  std::is_same_v<T, std::unique_ptr<Enemy>>) {
      sf::Vector2f pos = creature->GetHitbox().getPosition();
      sf::Vector2f size = creature->GetHitbox().getSize();
      sf::Vector2f borders_pos = borders_.getPosition();
      sf::Vector2f borders_size = borders_.getSize();
      if (pos.x + size.x / 2 < borders_pos.x) {
        pos.x = borders_pos.x;
      } else if (pos.x + size.x / 2 > borders_pos.x + borders_size.x) {
        pos.x = borders_pos.x + borders_size.x - size.x;
      }
      if (pos.y + size.y / 2 < borders_pos.y) {
        pos.y = borders_pos.y;
      } else if (pos.y + size.y / 2 > borders_pos.y + borders_size.y) {
        pos.y = borders_pos.y + borders_size.y - size.y;
      }
      creature->SetPosition(pos);
    }
  }

  std::vector<std::vector<std::vector<std::unique_ptr<Tile>>>> map_;
  static int grid_size_;
  sf::Vector2i world_size_;
  sf::RectangleShape borders_;
};

template <typename T>
void TileMap::UpdateCreature(T &creature, float time_elapsed) {
  if constexpr (std::is_same_v<T, std::shared_ptr<Creature>> ||
                std::is_same_v<T, std::unique_ptr<Creature>> ||
                std::is_same_v<T, std::unique_ptr<Enemy>>) {
    CheckBorders(creature);
    sf::RectangleShape hitbox = creature->GetHitbox();
    sf::Vector2f left_up_pos = hitbox.getPosition();
    sf::Vector2f speed = creature->GetPhysicsComponent()->GetLastVelocity();
    left_up_pos = {left_up_pos.x +
                       move_const::kTimeNormalizerMap * speed.x * time_elapsed,
                   left_up_pos.y +
                       move_const::kTimeNormalizerMap * speed.y * time_elapsed};
    sf::Vector2f size = hitbox.getSize();
    sf::Vector2f borders_pos = borders_.getPosition();
    sf::Vector2f borders_size = borders_.getSize();
    PossibleDirections new_directions;
    new_directions = {left_up_pos.x >= borders_pos.x,
                      (left_up_pos.x + size.x) <=
                          borders_pos.x + borders_size.x,
                      left_up_pos.y >= borders_pos.y,
                      left_up_pos.y + size.y <= borders_pos.y + borders_size.y};
    PossibleDirections tile_directions;
    tile_directions.left = CanMove(utility::CreateSideRect(
        left_up_pos,
        {left_up_pos.x - move_const::kSmallValue, left_up_pos.y + size.y}));
    tile_directions.right = CanMove(utility::CreateSideRect(
        {left_up_pos.x + size.x, left_up_pos.y},
        {left_up_pos.x + size.x + move_const::kSmallValue,
         left_up_pos.y + size.y}));
    tile_directions.up = CanMove(utility::CreateSideRect(
        left_up_pos,
        {left_up_pos.x + size.x, left_up_pos.y - move_const::kSmallValue}));
    tile_directions.down = CanMove(utility::CreateSideRect(
        {left_up_pos.x, left_up_pos.y + size.y},
        {left_up_pos.x + size.x,
         left_up_pos.y + size.y + move_const::kSmallValue}));
    new_directions = new_directions && tile_directions;
    creature->GetPhysicsComponent()->SetPossibleMoveDirections(new_directions);
    sf::Vector2f creature_c_pos =
        creature->GetHitboxComponent()->GetCenteredPosition();
    sf::Vector2i grid_pos = GetGridPosition(creature_c_pos);
    double move_cost = 1;
    for (const auto &layer : map_.at(grid_pos.y).at(grid_pos.x)) {
      move_cost = std::max(move_cost, layer->GetMoveCost());
    }
    creature->GetPhysicsComponent()->UpdateSpeedMultiplier(1 / move_cost);
  }
}

#endif // ANTIPATTERNS_TILEMAP_H
