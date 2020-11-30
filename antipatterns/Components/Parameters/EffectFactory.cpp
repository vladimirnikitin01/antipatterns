//
// Created by dmitry on 5/16/20.
//

#include "EffectFactory.h"

using BaseStats = std::vector<std::shared_ptr<BaseAttribute>>;
using EffectFunc = std::function<void(Effect &, const BaseStats &)>;
using EffectFuncs = std::unordered_map<std::string, EffectFunc>;

const std::unordered_map<EFFECT_TYPE, EffectFuncs>
    EffectFactory::effect_type_to_func{
        {EFFECT_TYPE::PHYSICAL_DAMAGE,
         {{effect_const::kSelfKey, effects_formula::PhysDamageSelfFunction},
          {effect_const::kEnemyKey, effects_formula::PhysDamageEnemyFunction}}},
        {EFFECT_TYPE::MAGICAL_DAMAGE,
         {{effect_const::kSelfKey, effects_formula::MagDamageSelfFunction},
          {effect_const::kEnemyKey, effects_formula::MagDamageEnemyFunction}}},
        {EFFECT_TYPE::POISON,
         {{effect_const::kSelfKey, effects_formula::PoisonSelfFunction},
          {effect_const::kEnemyKey, effects_formula::PoisonEnemyFunction}}},
        {EFFECT_TYPE::STATS,
         {{effect_const::kSelfKey, effects_formula::StatsSelfFunction},
          {effect_const::kEnemyKey, effects_formula::StatsEnemyFunction}}}};

std::shared_ptr<Effect>
EffectFactory::CreateEffect(const std::map<std::string, Json::Node> &settings) {
  Stats stats{};
  LevelChange level_change{};
  ATTRIBUTE_ID curr_id =
      stat_const::names_to_id.at(settings.at("id").AsString());
  EFFECT_TYPE curr_type =
      effect_const::names_to_id.at(settings.at("type").AsString());
  stats.base_value = settings.at("stats").AsMap().at("base_value").AsDouble();
  stats.multiplier = settings.at("stats").AsMap().at("multiplier").AsDouble();
  level_change.delta_value =
      settings.at("level_change").AsMap().at("delta_value").AsDouble();
  level_change.multiplier =
      settings.at("level_change").AsMap().at("multiplier").AsDouble();
  std::shared_ptr<Effect> effect = nullptr;

  switch (curr_type) {
  case EFFECT_TYPE::STATS:
    effect = std::make_shared<TimedEffect>(curr_id, stats, level_change,
                                           &effect_type_to_func.at(curr_type));
    break;
  case EFFECT_TYPE::POISON:
    effect = std::make_shared<ProckingEffect>(
        curr_id, stats, level_change, &effect_type_to_func.at(curr_type));
    break;
  case EFFECT_TYPE::PHYSICAL_DAMAGE:
    effect = std::make_shared<OperatingEffect>(
        curr_id, stats, level_change, &effect_type_to_func.at(curr_type));
    break;
  case EFFECT_TYPE::MAGICAL_DAMAGE:
    effect = std::make_shared<OperatingEffect>(
        curr_id, stats, level_change, &effect_type_to_func.at(curr_type));
    break;
  }
  if (settings.count("optional") != 0) {
    effect->LoadFromMap(settings.at("optional").AsMap());
  }
  return effect;
}
