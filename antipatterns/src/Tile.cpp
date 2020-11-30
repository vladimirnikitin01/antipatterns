//
// Created by dmitry on 4/2/20.
//

#include "Tile.h"

Tile::Tile(TileType type, PlaceType p_type, const sf::Texture &texture,
           const sf::IntRect &rect, double move_cost, const sf::Vector2f &pos)
    : tile_type_(type), place_type_(p_type), sprite_(texture, rect),
      move_cost_(move_cost) {
  sprite_.setPosition(pos);
}

std::unique_ptr<Tile> Tile::Clone(const sf::Vector2f &position) const {
  auto new_tile = *this;
  new_tile.sprite_.setPosition(position);
  return std::make_unique<Tile>(std::move(new_tile));
}

double Tile::GetMoveCost() const { return move_cost_; }

PlaceType Tile::GetPlaceType() const { return place_type_; }

void Tile::Render(sf::RenderTarget &target) const { target.draw(sprite_); }
