//
// Created by dmitry on 4/25/20.
//

#include "Skill.h"
#include "EffectFactory.h"

void SkillVariables::LoadFromMap(
    const std::map<std::string, Json::Node> &settings) {
  cooldown = settings.at("cooldown").AsFloat();
  time_to_cast = cast_time = settings.at("cast_time").AsFloat();
  range = settings.at("range").AsFloat();
  priority_ = settings.at("priority").AsDouble();
  if (settings.count("type") != 0) {
    type_ = SkillType::SELF;
  }
  EffectFactory wow_factory;
  for (const auto &effect_map_json : settings.at("effects").AsArray()) {
    effects.push_back(wow_factory.CreateEffect(effect_map_json.AsMap()));
  }
}
SkillVariables::SkillVariables(const SkillVariables &other) { *this = other; }
SkillVariables &SkillVariables::operator=(const SkillVariables &other) {
  if (this == &other) {
    return *this;
  }
  cooldown = other.cooldown;
  cast_time = other.cast_time;
  range = other.range;
  time_to_cool = other.time_to_cool;
  time_to_cast = other.time_to_cast;
  priority_ = other.priority_;
  effects.clear();
  for (const auto &effect : other.effects) {
    effects.push_back(std::dynamic_pointer_cast<Effect>(effect->Clone()));
  }
  return *this;
}

Skill::Skill(const std::string &skill_key,
             const std::map<std::string, Json::Node> &settings) {
  animation_key_ = skill_key;
  status_ = SkillStatus::COOLING;
  variables_.LoadFromMap(settings);
}

void Skill::Cast() { status_ = SkillStatus::CAST; }

Skill::Skill(const Skill &other) { *this = other; }
Skill &Skill::operator=(const Skill &other) {
  if (this == &other) {
    return *this;
  }
  animation_key_ = other.animation_key_;
  status_ = other.status_;
  variables_ = other.variables_;
  return *this;
}

bool Skill::IsCasted() { return variables_.time_to_cast <= 0; }

bool Skill::IsOnCooldown() { return variables_.time_to_cool > 0; }

bool Skill::IsBeingCasted() { return status_ == SkillStatus::CAST; }

void Skill::UpdateAttributes(AttributeComponent *caster,
                             AttributeComponent *receiver) {
  for (const std::shared_ptr<Effect> &effect : variables_.effects) {
    std::shared_ptr<Effect> copy =
        std::dynamic_pointer_cast<Effect>(effect->Clone());
    copy->UpdateAttributes(effect_const::kSelfKey, caster->GetAttributes());
    // check if caster and receiver are enemies
    if (caster != receiver) {
      copy->UpdateAttributes(effect_const::kEnemyKey,
                             receiver->GetAttributes());
    }
    receiver->AddEffect(copy);
  }
}

void Skill::UpdateCool(float time_elapsed) {
  if (variables_.time_to_cool > 0) {
    variables_.time_to_cool -= time_elapsed;
  }
  if (variables_.time_to_cool < 0) {
    variables_.time_to_cool = 0;
  }
}

void Skill::UpdateCast(float time_elapsed) {
  if (variables_.time_to_cast > 0) {
    variables_.time_to_cast -= time_elapsed;
  }
  if (variables_.time_to_cast < 0) {
    variables_.time_to_cast = 0;
  }
}

void Skill::Update(float time_elapsed) {
  (IsBeingCasted()) ? UpdateCast(time_elapsed) : Reset();
  if (IsOnCooldown()) {
    UpdateCool(time_elapsed);
  }
}

void Skill::Reset() {
  if (status_ == SkillStatus::CAST) {
    if (IsCasted()) {
      variables_.time_to_cool = variables_.cooldown;
    }
    variables_.time_to_cast = variables_.cast_time;
    status_ = SkillStatus::COOLING;
  } else if (variables_.time_to_cast <= 0) {
    variables_.time_to_cool = variables_.cooldown;
  }
}

std::string Skill::GetAnimationKey() const { return animation_key_; }

double Skill::GetPriority() const { return variables_.priority_; }
SkillType Skill::GetType() const { return variables_.type_; }
const SkillVariables &Skill::GetAllData() const { return variables_; }
