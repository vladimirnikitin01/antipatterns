//
// Created by dmitry on 4/2/20.
//

#ifndef ANTIPATTERNS_RAT_H
#define ANTIPATTERNS_RAT_H

#include "../src/Enemy.h"

class Rat : public Enemy {
public:
  Rat(const sf::Texture &texture_sheet,
      const std::map<std::string, Json::Node> &settings);
  Rat(const Rat &other);
  ~Rat() = default;

  void Update(float time_elapsed) override;
  void Render(sf::RenderTarget &target) const override;
  std::unique_ptr<Enemy> Clone() const override;

private:
  void UpdateAnimations(float time_elapsed);
};

#endif // ANTIPATTERNS_RAT_H
