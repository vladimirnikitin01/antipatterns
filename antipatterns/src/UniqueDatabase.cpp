//
// Created by dmitry on 5/2/20.
//

#include "UniqueDatabase.h"

UniqueDatabase::UniqueDatabase() { LoadUniqueData(file::kUniqueDataFile); }

void UniqueDatabase::LoadUniqueData(const std::string &file_name) {
  const std::map<std::string, Json::Node> unique_data =
      Json::Load(file_name).GetRoot().AsMap();
  LoadTextures(unique_data.at("textures").AsString());
  LoadEnemies(unique_data.at("enemies").AsString());
  LoadExits(unique_data.at("exits").AsString());
  LoadTiles(unique_data.at("tiles").AsString());
}

void UniqueDatabase::LoadTextures(const std::string &file_name) {
  const std::map<std::string, Json::Node> textures =
      Json::Load(file_name).GetRoot().AsMap();
  for (const auto &[texture_name, texture_file_node] : textures) {
    if (!unique_data_.textures[texture_name].loadFromFile(
            texture_file_node.AsString())) {
      throw std::runtime_error(texture_name + "texture load error");
    }
  }
}

const UniqueData &UniqueDatabase::GetData() const { return unique_data_; }

void UniqueDatabase::LoadEnemies(const std::string &file_name) {
  const std::map<std::string, Json::Node> enemy_settings =
      Json::Load(file_name).GetRoot().AsMap();
  AddUniqueEnemy(EnemyType::ENEMY_SPAWNER,
                 std::make_shared<EnemySpawner>(
                     unique_data_.textures.at("EnemySpawner"),
                     enemy_settings.at("EnemySpawner").AsMap()));

  AddUniqueEnemy(EnemyType::RAT,
                 std::make_shared<Rat>(unique_data_.textures.at("Rat"),
                                       enemy_settings.at("Rat").AsMap()));
}

void UniqueDatabase::LoadTiles(const std::string &file_name) {
  const std::map<std::string, Json::Node> settings =
      Json::Load(file_name).GetRoot().AsMap();
  int grid_size = static_cast<int>(settings.at("grid_size").AsDouble());
  TileMap::SetGridSize(grid_size);
  for (const auto &settings_map_node : settings.at("tiles").AsArray()) {
    const auto &settings_map = settings_map_node.AsMap();
    sf::IntRect curr_rect;
    TileType curr_type;
    PlaceType place_type;
    double cost{};
    curr_rect.height = curr_rect.width = TileMap::GetGridSize();
    curr_rect.left =
        static_cast<int>(settings_map.at("x").AsDouble()) * grid_size;
    curr_rect.top =
        static_cast<int>(settings_map.at("y").AsDouble()) * grid_size;
    curr_type = static_cast<TileType>(settings_map.at("type").AsDouble());
    place_type = static_cast<PlaceType>(settings_map.at("place_type").AsInt());
    cost = settings_map.at("cost").AsDouble();
    unique_data_.tiles.try_emplace(
        curr_type, std::make_unique<Tile>(curr_type, place_type,
                                          unique_data_.textures.at("TILE_MAP"),
                                          curr_rect, cost));
  }
}

void UniqueDatabase::LoadExits(const std::string &file_name) {
  const std::map<std::string, Json::Node> settings =
      Json::Load(file_name).GetRoot().AsMap();
  const std::vector<Json::Node> &exits_nodes = settings.at("exits").AsArray();
  for (const auto &exit_node : exits_nodes) {
    const auto &exit_settings = exit_node.AsMap();
    sf::IntRect curr_rect{};
    ExitType exit_type;
    curr_rect.height = static_cast<int>(exit_settings.at("height").AsDouble());
    curr_rect.width = static_cast<int>(exit_settings.at("width").AsDouble());
    curr_rect.left = static_cast<int>(exit_settings.at("x").AsDouble());
    curr_rect.top = static_cast<int>(exit_settings.at("y").AsDouble());
    float exit_hitbox_width = exit_settings.at("hitbox_x").AsFloat();
    float exit_hitbox_height = exit_settings.at("hitbox_y").AsFloat();

    exit_type = static_cast<ExitType>(exit_settings.at("type").AsDouble());
    unique_data_.exits.try_emplace(
        exit_type,
        std::make_unique<Exit>(sf::RectangleShape(sf::Vector2f(
                                   exit_hitbox_width, exit_hitbox_height)),
                               unique_data_.textures.at("EXITS"), curr_rect));
  }
}

void UniqueDatabase::AddUniqueEnemy(EnemyType enemy_type,
                                    std::shared_ptr<Enemy> enemy) {
  if (!unique_data_.enemies.try_emplace(enemy_type, enemy).second) {
    throw std::runtime_error("You didn't load all enemies: " +
                             std::to_string(static_cast<int>(enemy_type)) +
                             " enemy fails");
  }
}
