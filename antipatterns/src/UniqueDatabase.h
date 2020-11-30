//
// Created by dmitry on 5/2/20.
//

#ifndef ANTIPATTERNS_UNIQUEDATABASE_H
#define ANTIPATTERNS_UNIQUEDATABASE_H

#include "../Enemies/Enemies.h"
#include "../Resources/Constants/Filenames.h"
#include "Enemy.h"
#include "Exit.h"
#include "TileMap.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>

enum class TileType;
enum class ExitType;
enum class EnemyType;
enum class ROOM_ID;
enum class ATTRIBUTE_ID;
class Exit;
class Tile;
class Enemy;

struct UniqueData {
  std::unordered_map<ExitType, std::unique_ptr<Exit>> exits;
  std::unordered_map<TileType, std::unique_ptr<Tile>> tiles;
  std::unordered_map<EnemyType, std::shared_ptr<Enemy>> enemies;
  std::unordered_map<std::string, sf::Texture> textures;
};

class UniqueDatabase {
public:
  static const UniqueDatabase &Instance() {
    static UniqueDatabase only_instance;
    return only_instance;
  }

  const UniqueData &GetData() const;

  UniqueDatabase(const UniqueDatabase &other) = delete;
  UniqueDatabase &operator=(const UniqueDatabase &other) = delete;
  UniqueDatabase(UniqueDatabase &&other) = delete;
  UniqueDatabase &operator=(UniqueDatabase &&other) = delete;

private:
  UniqueDatabase();
  void LoadUniqueData(const std::string &file_name);
  void LoadTextures(const std::string &file_name);
  void LoadEnemies(const std::string &file_name);
  void LoadTiles(const std::string &file_name);
  void LoadExits(const std::string &file_name);

  void AddUniqueEnemy(EnemyType enemy_type, std::shared_ptr<Enemy> enemy);
  UniqueData unique_data_;
};

#endif // ANTIPATTERNS_UNIQUEDATABASE_H
