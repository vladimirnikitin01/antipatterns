//
// Created by dmitry on 4/2/20.
//

#include "TileMap.h"

int TileMap::grid_size_ = 62;

TileMap::TileMap(
    const std::string &map_tile_name,
    const std::unordered_map<TileType, std::unique_ptr<Tile>> &unique_tiles) {
  InitMap(map_tile_name, unique_tiles);
}

void TileMap::Render(sf::RenderTarget &target) const {
  for (auto &vec_x : map_) {
    for (auto &vec_y : vec_x) {
      for (auto &tile_layer : vec_y) {
        tile_layer->Render(target);
      }
    }
  }
}

int TileMap::GetGridSize() { return grid_size_; }

void TileMap::SetGridSize(int grid_size) { grid_size_ = grid_size; }

sf::Vector2i TileMap::GetGridPosition(sf::Vector2f global_position) const {
  return sf::Vector2i(
      static_cast<int>(static_cast<int>(global_position.x) / grid_size_),
      static_cast<int>(static_cast<int>(global_position.y)) / grid_size_);
}

sf::Vector2f TileMap::GetGlobalPosition(sf::Vector2i grid_position) const {
  return sf::Vector2f(static_cast<float>(grid_position.x * grid_size_),
                      static_cast<float>(grid_position.y * grid_size_));
}

std::vector<std::vector<int>> TileMap::GetCostMap() const {
  std::vector<std::vector<int>> cost_map(
      map_.at(0).size(), std::vector<int>(map_.size(), -1));
  for (int j = 0; j < map_.at(0).size(); ++j) {
    for (int i = 0; i < map_.size(); ++i) {
      for (const auto &layer : map_.at(i).at(j)) {
        if (layer->GetPlaceType() != PlaceType::SOLID) {
          cost_map.at(j).at(i) = std::max(
              static_cast<int>(layer->GetMoveCost()), cost_map.at(j).at(i));
        }
      }
    }
  }
  return cost_map;
}

/* private */

void TileMap::InitMap(
    const std::string &file_name,
    const std::unordered_map<TileType, std::unique_ptr<Tile>> &unique_tiles) {
  const std::map<std::string, Json::Node> map_settings =
      Json::Load(file_name).GetRoot().AsMap();
  const std::vector<Json::Node> shift_nodes =
      map_settings.at("shift").AsArray();
  sf::Vector2f shift =
      sf::Vector2f(shift_nodes.at(0).AsFloat(), shift_nodes.at(1).AsFloat());
  auto borders_map = map_settings.at("borders").AsMap();
  borders_.setPosition(borders_map.at("pos_x").AsFloat(),
                       borders_map.at("pos_y").AsFloat());
  borders_.setSize(
      {borders_map.at("width").AsFloat(), borders_map.at("height").AsFloat()});
  const std::vector<Json::Node> map_layers = map_settings.at("map").AsArray();
  world_size_.y = map_layers.empty() ? 0 : map_layers.front().AsArray().size();
  world_size_.x = world_size_.y == 0
                      ? 0
                      : map_layers.front().AsArray().front().AsArray().size();
  //настраиваем размеры карты
  map_.resize(world_size_.y);
  for (auto &sub_vec : map_) {
    sub_vec.resize(world_size_.x);
  }
  for (const auto &layer_node : map_layers) {
    const std::vector<Json::Node> &layer = layer_node.AsArray();
    for (int y_pos = 0; y_pos < world_size_.y; ++y_pos) {
      const std::vector<Json::Node> &row = layer[y_pos].AsArray();
      for (int x_pos = 0; x_pos < world_size_.x; ++x_pos) {
        auto type = static_cast<TileType>(row[x_pos].AsDouble());
        AddTileLayer(type, x_pos, y_pos, shift, unique_tiles);
      }
    }
  }
  auto vec = GetCostMap();
}

void TileMap::AddTileLayer(
    TileType type, int x_pos, int y_pos, sf::Vector2f shift,
    const std::unordered_map<TileType, std::unique_ptr<Tile>> &unique_tiles) {
  if (type != TileType::NOTHING) {
    sf::Vector2f pos = GetGlobalPosition(sf::Vector2i(x_pos, y_pos));
    map_.at(y_pos).at(x_pos).push_back(unique_tiles.at(type)->Clone(
        sf::Vector2f(pos.x + shift.x, pos.y + shift.y)));
  }
}

bool TileMap::CanMove(const sf::FloatRect &rect) {
  sf::Vector2f left_top_pos = {rect.left, rect.top};
  sf::Vector2f right_down_pos = {left_top_pos.x + rect.width,
                                 left_top_pos.y + rect.height};
  sf::Vector2i left_top_grid = GetGridPosition(left_top_pos);
  sf::Vector2i right_down_grid = GetGridPosition(right_down_pos);
  for (int j = left_top_grid.y; j <= right_down_grid.y; ++j) {
    for (int i = left_top_grid.x; i <= right_down_grid.x; ++i) {
      for (const auto &layer : map_.at(j).at(i)) {
        if (layer->GetPlaceType() == PlaceType::SOLID) {
          return false;
        }
      }
    }
  }
  return true;
}
