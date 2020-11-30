//
// Created by dmitry on 4/1/20.
//

#ifndef ANTIPATTERNS_HITBOXCOMPONENT_H
#define ANTIPATTERNS_HITBOXCOMPONENT_H

#include "../Resources/Json.h"
#include <SFML/Graphics.hpp>

class HitboxComponent {
public:
  HitboxComponent(sf::Sprite &sprite,
                  const std::map<std::string, Json::Node> &settings);
  HitboxComponent(const HitboxComponent &other) = default;
  HitboxComponent &operator=(const HitboxComponent &other) = default;
  ~HitboxComponent();

  /* initializer */
  void LoadFromMap(const std::map<std::string, Json::Node> &settings);

  void Update();
  void Render(sf::RenderTarget &target) const;

  void SetPosition(const sf::Vector2f &position);
  void UpdateCopy(sf::Sprite &sprite);

  sf::RectangleShape GetHitbox() const;

  sf::Vector2f GetPosition() const;
  sf::Vector2f GetCenteredPosition() const;

  bool Intersects(const sf::RectangleShape &other_hitbox) const;
  bool Contains(const sf::RectangleShape &other_hitbox) const;
  bool ContainedIn(const sf::RectangleShape &other_hitbox) const;

private:
  sf::RectangleShape hitbox_;
  sf::Sprite *sprite_;
  sf::Vector2f offset_;
};

#endif // ANTIPATTERNS_HITBOXCOMPONENT_H
