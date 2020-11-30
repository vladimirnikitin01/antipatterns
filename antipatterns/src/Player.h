//
// Created by dmitry on 3/30/20.
//

#ifndef ANTIPATTERNS_PLAYER_H
#define ANTIPATTERNS_PLAYER_H

#include "Creature.h"

class Player : public Creature {
public:
  Player(sf::Vector2f position, const sf::Texture &texture_sheet,
         const std::string &file_name);
  ~Player() override;

  void Update(float time_elapsed) override;
  void Render(sf::RenderTarget &target) const override;

private:
  void UpdateAnimations(float time_elapsed);

  sf::Vector2f position_;
};

#endif // ANTIPATTERNS_PLAYER_H
