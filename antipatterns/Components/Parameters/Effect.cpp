//
// Created by dmitry on 4/21/20.
//

#include "Effect.h"

/// ---------------- EFFECT ------------------- ///

using BaseStats = std::vector<std::shared_ptr<BaseAttribute>>;
using EffectFunc = std::function<void(Effect &, const BaseStats &)>;

Effect::Effect(ATTRIBUTE_ID id, Stats stats, LevelChange level_change,
               const EffectFuncs *effect_funcs)
    : BaseAttribute(stats, level_change), id_(id), effect_funcs_(effect_funcs) {
}

void Effect::SetAttributeId(ATTRIBUTE_ID id) { id_ = id; }

ATTRIBUTE_ID Effect::GetAttributeid() const { return id_; }

std::shared_ptr<BaseAttribute> Effect::Clone() const {
  std::shared_ptr<Effect> copy =
      std::make_shared<Effect>(id_, stats_, level_change_, effect_funcs_);
  for (const auto &raw_bonus : raw_bonuses_) {
    copy->AddRawBonus(raw_bonus->Clone());
  }
  for (const auto &effect : effects_) {
    effect->AddEffect(effect->Clone());
  }
  copy->current_value_ = current_value_;
  copy->updated_ = updated_;
  return copy;
}
void Effect::SetMultiplier(double multiplier) {
  stats_.multiplier = multiplier;
}
void Effect::LoadFromMap(const std::map<std::string, Json::Node> &settings) {}

void Effect::UpdateAttributes(const std::string &update_key,
                              const BaseStats &stats) {
  if (effect_funcs_->count(update_key) != 0) {
    effect_funcs_->at(update_key)(*this, stats);
  }
}

/// ---------------- OPERATING EFFECT ------------- ///

OperatingEffect::OperatingEffect(ATTRIBUTE_ID id, Stats stats,
                                 LevelChange level_change,
                                 const EffectFuncs *effect_funcs)
    : Effect(id, stats, level_change, effect_funcs) {}

std::shared_ptr<BaseAttribute> OperatingEffect::Clone() const {
  std::shared_ptr<OperatingEffect> copy = std::make_shared<OperatingEffect>(
      id_, stats_, level_change_, effect_funcs_);
  for (const auto &raw_bonus : raw_bonuses_) {
    copy->AddRawBonus(raw_bonus->Clone());
  }
  for (const auto &effect : effects_) {
    effect->AddEffect(effect->Clone());
  }
  copy->current_value_ = current_value_;
  copy->updated_ = updated_;
  copy->SetToRemove(to_remove_);
  return copy;
}

bool OperatingEffect::ToRemove() const { return to_remove_; }

void OperatingEffect::SetToRemove(bool value) { to_remove_ = value; }

bool OperatingEffect::IsReady() const {
  to_remove_ = true;
  return true;
}
void OperatingEffect::LoadFromMap(
    const std::map<std::string, Json::Node> &settings) {}

/// -----------------TIMED EFFECT ------------------ ///

TimedEffect::TimedEffect(ATTRIBUTE_ID id, Stats stats, LevelChange level_change,
                         const EffectFuncs *effect_funcs)
    : Effect(id, stats, level_change, effect_funcs) {}

std::shared_ptr<BaseAttribute> TimedEffect::Clone() const {
  std::shared_ptr<TimedEffect> copy =
      std::make_shared<TimedEffect>(id_, stats_, level_change_, effect_funcs_);
  for (const auto &raw_bonus : raw_bonuses_) {
    copy->AddRawBonus(raw_bonus->Clone());
  }
  for (const auto &effect : effects_) {
    effect->AddEffect(effect->Clone());
  }
  copy->SetExpirationTime(time_to_expire_);
  copy->updated_ = updated_;
  copy->current_value_ = current_value_;
  return copy;
}

bool TimedEffect::ToRemove() const { return time_to_expire_ <= 0; }

void TimedEffect::Update(float time_elapsed) {
  if (updated_) {
    return;
  }
  time_to_expire_ -= time_elapsed;
  updated_ = true;
  current_value_ = stats_.base_value;
  UpdateBonuses(time_elapsed, raw_bonuses_);
  UpdateBonuses(time_elapsed, effects_);
}

void TimedEffect::SetExpirationTime(float time_to_expire) {
  time_to_expire_ = time_to_expire;
}
void TimedEffect::LoadFromMap(
    const std::map<std::string, Json::Node> &settings) {
  if (settings.count("time_to_expire") != 0) {
    time_to_expire_ = settings.at("time_to_expire").AsFloat();
  }
}

/// --------------OVER_TIME_EFFECT ------------///

OverTimeEffect::OverTimeEffect(ATTRIBUTE_ID id, Stats stats,
                               LevelChange level_change,
                               const EffectFuncs *effect_funcs)
    : TimedEffect(id, stats, level_change, effect_funcs) {}

std::shared_ptr<BaseAttribute> OverTimeEffect::Clone() const {
  std::shared_ptr<OverTimeEffect> copy = std::make_shared<OverTimeEffect>(
      id_, stats_, level_change_, effect_funcs_);
  for (const auto &raw_bonus : raw_bonuses_) {
    copy->AddRawBonus(raw_bonus->Clone());
  }
  for (const auto &effect : effects_) {
    effect->AddEffect(effect->Clone());
  }
  copy->SetExpirationTime(time_to_expire_);
  copy->current_value_ = current_value_;
  copy->updated_ = updated_;
  copy->SetTickTime(tick_time_);
  copy->SetTimeFromTick(time_from_tick_);
  return copy;
}

void OverTimeEffect::Update(float time_elapsed) {
  if (updated_) {
    return;
  }
  time_from_tick_ += time_elapsed;
  time_to_expire_ -= time_elapsed;
  updated_ = true;
  current_value_ = stats_.base_value;
  UpdateBonuses(time_elapsed, raw_bonuses_);
  UpdateBonuses(time_elapsed, effects_);
}

void OverTimeEffect::SetTickTime(float tick_time) { tick_time_ = tick_time; }

void OverTimeEffect::SetTimeFromTick(float time_from_tick) {
  time_from_tick_ = time_from_tick;
}
void OverTimeEffect::LoadFromMap(
    const std::map<std::string, Json::Node> &settings) {
  TimedEffect::LoadFromMap(settings);
  if (settings.count("tick_time") != 0) {
    tick_time_ = settings.at("tick_time").AsFloat();
  }
}

/// --------------- PROCKING_EFFECT ------------ ///

ProckingEffect::ProckingEffect(ATTRIBUTE_ID id, Stats stats,
                               LevelChange level_change,
                               const EffectFuncs *effect_funcs)
    : OverTimeEffect(id, stats, level_change, effect_funcs) {}

std::shared_ptr<BaseAttribute> ProckingEffect::Clone() const {
  std::shared_ptr<ProckingEffect> copy = std::make_shared<ProckingEffect>(
      id_, stats_, level_change_, effect_funcs_);
  for (const auto &raw_bonus : raw_bonuses_) {
    copy->AddRawBonus(raw_bonus->Clone());
  }
  for (const auto &effect : effects_) {
    effect->AddEffect(effect->Clone());
  }
  copy->SetExpirationTime(time_to_expire_);
  copy->current_value_ = current_value_;
  copy->updated_ = updated_;
  copy->SetTickTime(tick_time_);
  copy->SetTimeFromTick(time_from_tick_);
  return copy;
}

bool ProckingEffect::IsReady() const {
  if (time_from_tick_ >= tick_time_) {
    time_from_tick_ -= tick_time_;
    return true;
  }
  return false;
}
void ProckingEffect::LoadFromMap(
    const std::map<std::string, Json::Node> &settings) {
  OverTimeEffect::LoadFromMap(settings);
}
