//
// Created by dmitry on 3/28/20.
//

#ifndef ANTIPATTERNS_CREATURE_H
#define ANTIPATTERNS_CREATURE_H

#include "../Components/AttributeComponent.h"
#include "../Components/HitboxComponent.h"
#include "../Components/SkillComponent.h"
#include "../Resources/Utility.h"
#include "Tile.h"

class Creature {
public:
  Creature(const sf::Texture& texture_sheet, const std::map<std::string, Json::Node>& settings);
  Creature &operator=(const Creature &other);
  Creature(const Creature &other);

  virtual ~Creature();

  /* pure functions */
  virtual void Update(float time_elapsed) = 0;
  virtual void Render(sf::RenderTarget &target) const = 0;

  virtual void SetPosition(const sf::Vector2f &position);
  virtual void SetCenteredPosition(const sf::Vector2f &position);
  virtual void Move(float time_elapsed, const sf::Vector2f &direction);

  /* initializers */
  void SetTexture(sf::Texture &texture);
  void InitPhysicsComponent(const std::map<std::string, Json::Node> &settings);
  void InitGraphicsComponent(const sf::Texture &texture_sheet,
                             const std::map<std::string, Json::Node> &settings);
  void InitHitboxComponent(const std::map<std::string, Json::Node> &settings);
  void InitAttributeComponent(const std::map<std::string, Json::Node> &settings);
  void InitExpComponent(int level = 1);
  void InitSkillComponent(const std::map<std::string, Json::Node> &settings);

  /* getters */
  const std::unique_ptr<PhysicsComponent> &GetPhysicsComponent() const;
  const std::unique_ptr<GraphicsComponent> &GetGraphicsComponent() const;
  const std::unique_ptr<HitboxComponent> &GetHitboxComponent() const;
  const std::unique_ptr<AttributeComponent> &GetAttributeComponent() const;
  const std::unique_ptr<ExpComponent> &GetExpComponent() const;
  const std::unique_ptr<SkillComponent> &GetSkillComponent() const;

  sf::Vector2f GetPosition() const;
  sf::Vector2f GetCenteredPosition() const;
  sf::RectangleShape GetHitbox() const;
  float GetDistance(const Creature &other) const;
  bool IsDead() const;

protected:
  void UpdateMoveAnimations(float time_elapsed);

  std::unique_ptr<PhysicsComponent> phys_comp_{};
  std::unique_ptr<GraphicsComponent> graph_comp_{};
  std::unique_ptr<HitboxComponent> hitbox_comp_{};
  std::unique_ptr<AttributeComponent> attribute_comp_{};
  std::unique_ptr<ExpComponent> exp_comp_{};
  std::unique_ptr<SkillComponent> skill_comp_{};

  sf::Sprite sprite_{};

private:
};

#endif // ANTIPATTERNS_CREATURE_H
