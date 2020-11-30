//
// Created by dmitry on 4/2/20.
//

#ifndef ANTIPATTERNS_TILE_H
#define ANTIPATTERNS_TILE_H

#include "../Resources/GraphicsSettings.h"

enum class TileType {
  NOTHING = 0,
  BRICK = 1,
  SAND = 2,
  GRASS = 3,
  MUD = 4,
  YELLOW_BRICK = 5,
  WATER = 6,
  WATER_2 = 7
};

enum class PlaceType { NORMAL = 0, SOLID = 1 };

class Tile {
public:
  Tile(TileType type, PlaceType p_type, const sf::Texture &texture,
       const sf::IntRect &rect, double cost,
       const sf::Vector2f &pos = sf::Vector2f());

  std::unique_ptr<Tile> Clone(const sf::Vector2f &position) const;

  double GetMoveCost() const;

  PlaceType GetPlaceType() const;

  void Render(sf::RenderTarget &target) const;

private:
  sf::Sprite sprite_;
  TileType tile_type_;
  PlaceType place_type_;
  double move_cost_;
};

#endif // ANTIPATTERNS_TILE_H
