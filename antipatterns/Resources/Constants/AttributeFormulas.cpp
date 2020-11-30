//
// Created by dmitry on 4/29/20.
//

#include "AttributeFormulas.h"
#include "../../Components/Parameters/Effect.h"
#include "GamePlayConstants.h"

double stats_formula::MaxHPFunction(const stats_formula::BaseStats &stats) {
  return stat_const::kHpVigorCoeff *
         stats.at(ATTRIBUTE_ID::VIGOR)->GetCurrentValue();
}

double stats_formula::MaxManaFunction(const stats_formula::BaseStats &stats) {
  return stat_const::kManaIntelligenceCoeff *
             stats.at(ATTRIBUTE_ID::INTELLIGENCE)->GetCurrentValue() +
         stat_const::kManaFaithCoeff *
             stats.at(ATTRIBUTE_ID::FAITH)->GetCurrentValue();
}

double stats_formula::PhysArmorFunction(const stats_formula::BaseStats &stats) {
  return pow((stat_const::kPhysArmorVigorCoeff *
                  stats.at(ATTRIBUTE_ID::VIGOR)->GetCurrentValue() +
              stat_const::kPhysArmorStrengthCoeff *
                  stats.at(ATTRIBUTE_ID::STRENGTH)->GetCurrentValue() +
              stat_const::kPhysArmorDexterityCoeff *
                  stats.at(ATTRIBUTE_ID::DEXTERITY)->GetCurrentValue()) /
                 stat_const::kPhysArmorNormalizeCoeff,
             stat_const::kPhysArmorMainPow);
}

double stats_formula::MagArmorFunction(const stats_formula::BaseStats &stats) {
  return pow((stat_const::kMagArmorFaithCoeff *
                  stats.at(ATTRIBUTE_ID::FAITH)->GetCurrentValue() +
              stat_const::kMagArmorVigorCoeff *
                  stats.at(ATTRIBUTE_ID::VIGOR)->GetCurrentValue() +
              stat_const::kMagArmorIntelligenceCoeff *
                  stats.at(ATTRIBUTE_ID::INTELLIGENCE)->GetCurrentValue()) /
                 stat_const::kMagArmorNormalizeCoeff,
             stat_const::kMagArmorMainPow);
}

double
stats_formula::AttackSpeedFunction(const stats_formula::BaseStats &stats) {
  return stat_const::kAttackSpeedDexterityCoeff *
             stats.at(ATTRIBUTE_ID::DEXTERITY)->GetCurrentValue() +
         stat_const::kAttackSpeedStrengthCoeff *
             stats.at(ATTRIBUTE_ID::STRENGTH)->GetCurrentValue() +
         stat_const::kAttackSpeedIntelligenceCoeff *
             stats.at(ATTRIBUTE_ID::INTELLIGENCE)->GetCurrentValue();
}

double
stats_formula::BaseDamageFunction(const stats_formula::BaseStats &stats) {
  return (stat_const::kBaseDamageStrengthCoeff *
              stats.at(ATTRIBUTE_ID::STRENGTH)->GetCurrentValue() +
          stat_const::kBaseDamageDexterityCoeff *
              stats.at(ATTRIBUTE_ID::DEXTERITY)->GetCurrentValue()) /
         stat_const::kBaseDamageNormalizeCoeff;
}

double
stats_formula::CritChanceFunction(const stats_formula::BaseStats &stats) {
  return pow((stat_const::kCritChanceLuckCoeff *
                  stats.at(ATTRIBUTE_ID::LUCK)->GetCurrentValue() +
              stat_const::kCritChanceDexterityCoeff *
                  stats.at(ATTRIBUTE_ID::DEXTERITY)->GetCurrentValue()) /
                 stat_const::kCritChanceNormalizeCoeff,
             stat_const::kCritChanceMainPow);
}

#define Int(X) static_cast<int>(X)

void effects_formula::PhysDamageSelfFunction(
    Effect &effect, const effects_formula::BaseStats &stats) {
  double damage = effect.GetCurrentValue();
  double delta = stats.at(Int(ATTRIBUTE_ID::BASE_DAMAGE))->GetCurrentValue();
  effect.SetCurrentValue(damage - delta);
  if (CheckCrit(stats.at(Int(ATTRIBUTE_ID::CRIT_CHANCE))->GetCurrentValue())) {
    effect.SetCurrentValue(effect_const::kCritValueModifier *
                           effect.GetCurrentValue());
  }
}
void effects_formula::PhysDamageEnemyFunction(
    Effect &effect, const effects_formula::BaseStats &stats) {
  double armor = stats.at(Int(ATTRIBUTE_ID::PHYS_ARMOR))->GetCurrentValue();
  double damage = effect.GetCurrentValue();
  effect.SetCurrentValue(damage *
                         (1.0 - armor / effect_const::kArmorNormalizer));
}
void effects_formula::MagDamageSelfFunction(
    Effect &effect, const effects_formula::BaseStats &stats) {
  double damage = effect.GetCurrentValue();
  double delta = stats.at(Int(ATTRIBUTE_ID::BASE_DAMAGE))->GetCurrentValue();
  effect.SetCurrentValue(damage + delta);
  if (CheckCrit(stats.at(Int(ATTRIBUTE_ID::CRIT_CHANCE))->GetCurrentValue())) {
    effect.SetCurrentValue(effect_const::kCritValueModifier *
                           effect.GetCurrentValue());
  }
}
void effects_formula::MagDamageEnemyFunction(
    Effect &effect, const effects_formula::BaseStats &stats) {
  double armor = stats.at(Int(ATTRIBUTE_ID::MAG_ARMOR))->GetCurrentValue();
  double damage = effect.GetCurrentValue();
  effect.SetCurrentValue(damage *
                         (1.0 - armor / effect_const::kArmorNormalizer));
}
void effects_formula::PoisonSelfFunction(
    Effect &effect, const effects_formula::BaseStats &stats) {
  double delta =
      pow(stats.at(Int(ATTRIBUTE_ID::INTELLIGENCE))->GetCurrentValue() /
              effect_const::kSelfPoisonNormalizeCoeff,
          effect_const::kSelfPoisonMainPow);
  effect.SetCurrentValue((1.0 + delta) * effect.GetCurrentValue());
  effect.SetMultiplier((1.0 + delta) * effect.GetMultiplier());
}
void effects_formula::PoisonEnemyFunction(
    Effect &effect, const effects_formula::BaseStats &stats) {
  double delta = pow(stats.at(Int(ATTRIBUTE_ID::FAITH))->GetCurrentValue() /
                         effect_const::kEnemyPoisonNormalizeCoeff,
                     effect_const::kEnemyPoisonMainPow);
  effect.SetCurrentValue((1.0 - delta) * effect.GetCurrentValue());
  effect.SetMultiplier((1.0 - delta) * effect.GetMultiplier());
}
void effects_formula::StatsSelfFunction(
    Effect &effect, const effects_formula::BaseStats &stats) {
  double delta =
      pow(stats.at(Int(ATTRIBUTE_ID::INTELLIGENCE))->GetCurrentValue() /
              effect_const::kSelfStatsNormalizeCoeff,
          effect_const::kSelfStatsMainPow);
  effect.SetCurrentValue((1.0 + delta) * effect.GetCurrentValue());
}
void effects_formula::StatsEnemyFunction(
    Effect &effect, const effects_formula::BaseStats &stats) {
  double delta = pow(stats.at(Int(ATTRIBUTE_ID::FAITH))->GetCurrentValue() /
                         effect_const::kEnemyStatsNormalizeCoeff,
                     effect_const::kEnemyStatsMainPow);
  effect.SetCurrentValue((1.0 - delta) * effect.GetCurrentValue());
  effect.SetMultiplier((1.0 - delta) * effect.GetMultiplier());
}

bool effects_formula::CheckCrit(double crit_chance) {
  return RandomDevice::Instance().RandomNumber(0.0, 1.0) <=
         crit_chance / effect_const::kCritNormalizer;
}
