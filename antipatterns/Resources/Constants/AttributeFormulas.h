//
// Created by dmitry on 4/29/20.
//

#ifndef ANTIPATTERNS_ATTRIBUTEFORMULAS_H
#define ANTIPATTERNS_ATTRIBUTEFORMULAS_H

#include "../../Components/Parameters/BaseAttribute.h"
#include "../../Resources/RandomDevice.h"

class Effect;

namespace stats_formula {
using BaseStats =
    std::unordered_map<ATTRIBUTE_ID, std::shared_ptr<BaseAttribute>>;
using StatsFunc = std::function<double(
    const std::unordered_map<ATTRIBUTE_ID, std::shared_ptr<BaseAttribute>> &)>;

double MaxHPFunction(const BaseStats &stats);

double MaxManaFunction(const BaseStats &stats);

double PhysArmorFunction(const BaseStats &stats);

double MagArmorFunction(const BaseStats &stats);

double AttackSpeedFunction(const BaseStats &stats);

double BaseDamageFunction(const BaseStats &stats);

double CritChanceFunction(const BaseStats &stats);
} // namespace stats_formula

namespace effects_formula {
using BaseStats = std::vector<std::shared_ptr<BaseAttribute>>;
using EffectFunc =
    std::function<double(Effect&, const BaseStats &)>;

bool CheckCrit(double crit_chance);

void PhysDamageSelfFunction(Effect& effect, const BaseStats &stats);
void PhysDamageEnemyFunction(Effect& effect, const BaseStats& stats);

void MagDamageSelfFunction(Effect& effect, const BaseStats &stats);
void MagDamageEnemyFunction(Effect& effect, const BaseStats& stats);

void PoisonSelfFunction(Effect& effect, const BaseStats& stats);
void PoisonEnemyFunction(Effect& effect, const BaseStats& stats);

void StatsSelfFunction(Effect& effect, const BaseStats& stats);
void StatsEnemyFunction(Effect& effect, const BaseStats& stats);

} // namespace effects_formula

#endif // ANTIPATTERNS_ATTRIBUTEFORMULAS_H
