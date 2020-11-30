//
// Created by dmitry on 4/25/20.
//

#ifndef ANTIPATTERNS_SKILLCOMPONENT_H
#define ANTIPATTERNS_SKILLCOMPONENT_H

#include "Parameters/Skill.h"

class SkillComponent {
public:
  SkillComponent(const std::map<std::string, Json::Node>& settings);

  SkillComponent(const SkillComponent& other);

  SkillComponent& operator = (const SkillComponent& other);

  void Update(float time_elapsed);

  Skill *GetBestSkill(float min_range);
  Skill *GetSkill(const std::string& skill_key);

private:
  void LoadFromMap(const std::map<std::string, Json::Node>& settings);

  std::unordered_map<std::string, std::unique_ptr<Skill>> skills_;
};


#endif //ANTIPATTERNS_SKILLCOMPONENT_H
