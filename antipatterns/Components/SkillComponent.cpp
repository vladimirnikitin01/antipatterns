//
// Created by dmitry on 4/25/20.
//

#include "SkillComponent.h"

SkillComponent::SkillComponent(
    const std::map<std::string, Json::Node> &settings) {
  LoadFromMap(settings);
}

void SkillComponent::LoadFromMap(
    const std::map<std::string, Json::Node> &settings) {
  for (const auto &[key, skill_settings_node] : settings) {
    skills_[key] = std::make_unique<Skill>(key, skill_settings_node.AsMap());
  }
}
SkillComponent::SkillComponent(const SkillComponent &other) { *this = other; }

void SkillComponent::Update(float time_elapsed) {
  for (auto &[key, skill] : skills_) {
    skill->Update(time_elapsed);
  }
}

SkillComponent &SkillComponent::operator=(const SkillComponent &other) {
  if (&other == this) {
    return *this;
  }
  skills_.clear();
  for (const auto &[key, skill_ptr] : other.skills_) {
    skills_[key] = std::make_unique<Skill>(*skill_ptr);
  }
  return *this;
}

Skill *SkillComponent::GetBestSkill(float min_range) {
  double priority = -1 * std::numeric_limits<double>::infinity();
  Skill *result = nullptr;
  for (const auto &[key, skill] : skills_) {
    if (skill->GetPriority() > priority && !skill->IsOnCooldown() &&
        skill->GetAllData().range >= min_range) {
      priority = skill->GetPriority();
      result = skill.get();
    }
    if (skill->IsBeingCasted()) {
      return skill.get();
    }
  }
  return result;
}
Skill *SkillComponent::GetSkill(const std::string &skill_key) {
  return skills_.at(skill_key).get();
}
