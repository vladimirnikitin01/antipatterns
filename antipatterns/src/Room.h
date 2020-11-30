//
// Created by dmitry on 4/30/20.
//

#ifndef ANTIPATTERNS_ROOM_H
#define ANTIPATTERNS_ROOM_H

#include "EnemySystem.h"
#include "Input/Message.h"
#include "Player.h"
#include "UniqueDatabase.h"

class Room {
public:
  explicit Room(const std::string &file_name);

  void Update(float time_elapsed);

  void Render(sf::RenderTarget &target);

  void ReceiveMessage(std::unique_ptr<message::Message> message);

  // nullptr if no exit, else ptr to first active exit found
  const Exit *GetActiveExit() const;

  ROOM_ID GetRoomID() const;

  void AddExit(std::unique_ptr<Exit> exit);

  void SetPlayer(std::shared_ptr<Creature> player);

private:
  void InitEnemySystem(const std::map<std::string, Json::Node> &enemy_settings);
  void InitTileMap(const std::map<std::string, Json::Node> &map_settings);

  void UpdateCollisions(float time_elapsed);

  std::shared_ptr<Creature> player_{nullptr};
  std::unique_ptr<TileMap> map_{nullptr};
  std::unique_ptr<EnemySystem> enemy_system_{nullptr};
  std::vector<std::unique_ptr<Exit>> exits_;
  ROOM_ID room_id_{};
};

#endif // ANTIPATTERNS_ROOM_H
