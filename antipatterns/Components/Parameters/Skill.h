//
// Created by dmitry on 4/25/20.
//

#ifndef ANTIPATTERNS_SKILL_H
#define ANTIPATTERNS_SKILL_H

#include "../AttributeComponent.h"
#include "Attribute.h"
#include "Effect.h"

/*класс для реализации простейшей механики способности,
 * никак не для способности in general */

enum class SkillStatus { COOLING, CAST };

enum class SkillType { SELF, ENEMY };

struct SkillVariables {
  float cooldown{0};
  float cast_time{0};
  float range{10};
  float time_to_cool{0};
  float time_to_cast{0};
  double priority_{};
  SkillType type_{SkillType::ENEMY};
  std::vector<std::shared_ptr<Effect>> effects;
  SkillVariables() = default;
  SkillVariables(const SkillVariables &other);
  SkillVariables &operator=(const SkillVariables &other);
  void LoadFromMap(const std::map<std::string, Json::Node> &settings);
};

class Skill {
public:
  Skill(const std::string &skill_key,
        const std::map<std::string, Json::Node> &settings);

  Skill(const Skill &other);

  Skill &operator=(const Skill &other);

  void UpdateAttributes(AttributeComponent *caster,
                        AttributeComponent *receiver);

  bool IsOnCooldown();

  bool IsBeingCasted();

  bool IsCasted();

  [[nodiscard]] std::string GetAnimationKey() const;
  [[nodiscard]] double GetPriority() const;
  [[nodiscard]] SkillType GetType() const;
  [[nodiscard]] const SkillVariables& GetAllData() const;

  void Update(float time_elapsed);

  void UpdateCool(float time_elapsed);

  void UpdateCast(float time_elapsed);

  void Cast();

  void Reset();

private:
  std::string animation_key_{};
  SkillStatus status_;
  SkillVariables variables_{};
};

#endif // ANTIPATTERNS_SKILL_H
