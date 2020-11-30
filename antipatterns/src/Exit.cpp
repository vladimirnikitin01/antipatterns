//
// Created by dmitry on 4/30/20.
//

#include "Exit.h"
using namespace sf;

Exit::Exit(sf::RectangleShape hitbox, const Texture &texture_sheet,
           sf::IntRect texture_rect) {
  hitbox_ = std::move(hitbox);
  sprite_.setTexture(texture_sheet);
  sprite_.setTextureRect(texture_rect);
}

void Exit::Render(sf::RenderTarget &target) const { target.draw(sprite_); }

ROOM_ID Exit::GetRoomIdFrom() const { return id_from_; }

ROOM_ID Exit::GetRoomIdTo() const { return id_to_; }

const RectangleShape &Exit::GetHitbox() const { return hitbox_; }

sf::Vector2f Exit::GetPlayerPos() const { return player_pos_; }

void Exit::LoadFromMap(const std::map<std::string, Json::Node> &settings) {
  sf::Vector2f pos{}, offset{};
  pos.x = static_cast<float>(settings.at("pos_x").AsDouble());
  pos.y = static_cast<float>(settings.at("pos_y").AsDouble());
  offset.x = static_cast<float>(settings.at("offset_x").AsDouble());
  offset.y = static_cast<float>(settings.at("offset_y").AsDouble());
  id_from_ = static_cast<ROOM_ID>(settings.at("id_from").AsDouble());
  id_to_ = static_cast<ROOM_ID>(settings.at("id_to").AsDouble());
  player_pos_.x = settings.at("player_pos_x").AsFloat();
  player_pos_.y = settings.at("player_pos_y").AsFloat();
  sprite_.setPosition(pos.x, pos.y);
  hitbox_.setPosition(pos.x + offset.x, pos.y + offset.y);
}

sf::Vector2f Exit::GetPosition() const { return hitbox_.getPosition(); }

Exit::Exit(ROOM_ID from, ROOM_ID to) : id_from_(from), id_to_(to) {}

void Exit::SetRoomIDs(ROOM_ID from, ROOM_ID to) {
  id_from_ = from;
  id_to_ = to;
}
