//
// Created by dmitry on 5/4/20.
//

#ifndef ANTIPATTERNS_GAMEPLAYCONSTANTS_H
#define ANTIPATTERNS_GAMEPLAYCONSTANTS_H

#include "../../Components/Parameters/BaseAttribute.h"
#include <string>
#include <unordered_map>
#include <vector>

enum class EFFECT_TYPE;

namespace room_const {
constexpr float kInitPlayerX = 0;
constexpr float kInitPlayerY = 0;

constexpr int kSolidTileValue = -1;
constexpr double kReviveTime = 5;
constexpr double kSpawnDistance = 60;
constexpr float kSpawnTime = 10;
} // namespace room_const

namespace stat_const {
constexpr double kHpVigorCoeff = 10.0;

constexpr double kManaIntelligenceCoeff = 3.0;
constexpr double kManaFaithCoeff = 1.0;

constexpr double kPhysArmorVigorCoeff = 2.0;
constexpr double kPhysArmorStrengthCoeff = 1.0;
constexpr double kPhysArmorDexterityCoeff = 1.0;
constexpr double kPhysArmorNormalizeCoeff = 4.0;
constexpr double kPhysArmorMainPow = 0.7;

constexpr double kMagArmorFaithCoeff = 2.0;
constexpr double kMagArmorVigorCoeff = 1.0;
constexpr double kMagArmorIntelligenceCoeff = 1.0;
constexpr double kMagArmorNormalizeCoeff = 4.0;
constexpr double kMagArmorMainPow = 0.7;

constexpr double kAttackSpeedDexterityCoeff = 3.0;
constexpr double kAttackSpeedStrengthCoeff = 1.0;
constexpr double kAttackSpeedIntelligenceCoeff = 1.0;

constexpr double kBaseDamageStrengthCoeff = 5.0;
constexpr double kBaseDamageDexterityCoeff = 1.0;
constexpr double kBaseDamageNormalizeCoeff = 6.0;

constexpr double kCritChanceLuckCoeff = 5.0;
constexpr double kCritChanceDexterityCoeff = 1.0;
constexpr double kCritChanceNormalizeCoeff = 8.0;
constexpr double kCritChanceMainPow = 0.8;

constexpr int const kExpForNextLevel[11] = {0,    10,   50,   100,   200,  500,
                                            1000, 2000, 5000, 10000, 20000};
constexpr int const kMaxLevel = 10;

static const std::unordered_map<std::string, ATTRIBUTE_ID> names_to_id{
    {"vigor", ATTRIBUTE_ID::VIGOR},
    {"strength", ATTRIBUTE_ID::STRENGTH},
    {"dexterity", ATTRIBUTE_ID::DEXTERITY},
    {"intelligence", ATTRIBUTE_ID::INTELLIGENCE},
    {"faith", ATTRIBUTE_ID::FAITH},
    {"luck", ATTRIBUTE_ID::LUCK},
    {"max_hp", ATTRIBUTE_ID::MAX_HP},
    {"max_mana", ATTRIBUTE_ID::MAX_MANA},
    {"phys_armor", ATTRIBUTE_ID::PHYS_ARMOR},
    {"mag_armor", ATTRIBUTE_ID::MAG_ARMOR},
    {"attack_speed", ATTRIBUTE_ID::ATTACK_SPEED},
    {"base_damage", ATTRIBUTE_ID::BASE_DAMAGE},
    {"crit_chance", ATTRIBUTE_ID::CRIT_CHANCE},
    {"curr_hp", ATTRIBUTE_ID::CURR_HP},
    {"curr_mana", ATTRIBUTE_ID::CURR_MANA}};
} // namespace stat_const

namespace effect_const {
constexpr double kArmorNormalizer = 100.0;
constexpr double kCritNormalizer = 100.0;

static const std::string kSelfKey = "self";
static const std::string kEnemyKey = "enemy";

const std::unordered_map<std::string, EFFECT_TYPE> names_to_id{
    {"phys_damage", EFFECT_TYPE::PHYSICAL_DAMAGE},
    {"mag_damage", EFFECT_TYPE::MAGICAL_DAMAGE},
    {"poison", EFFECT_TYPE::POISON},
    {"stats", EFFECT_TYPE::STATS}};

constexpr double kCritValueModifier = 2.0;
constexpr double kSelfPoisonNormalizeCoeff = 100.0;
constexpr double kSelfPoisonMainPow = 1.2;

constexpr double kEnemyPoisonNormalizeCoeff = 100.0;
constexpr double kEnemyPoisonMainPow = 1.2;

constexpr double kSelfStatsNormalizeCoeff = 100.0;
constexpr double kEnemyStatsNormalizeCoeff = 100.0;

constexpr double kSelfStatsMainPow = 1.2;
constexpr double kEnemyStatsMainPow = 1.2;

} // namespace effect_const

namespace move_const {

constexpr float kHitboxRecSide = 0.8;
constexpr float kTimeNormalizerMap = 0.25;
constexpr float kSmallValue = 0.01;
constexpr float kLineStepValue = 0.25;
constexpr float kSmallDistance = 5;
constexpr float kSmallSpeed = 0.01;

} // namespace move_const

namespace command_const {
constexpr float kTeleportTime = 0.5;
constexpr float kTeleportDistance = 100;
constexpr float kTeleportCoolDown = 3;

static const std::string kHarakiriKey = "Harakiri";
static const std::string kAttackKey = "Attack";

static const std::string kAttackAnim = "ATTACK";

static const std::string kMoveLeftKey = "MOVE_LEFT";
static const std::string kMoveRightKey = "MOVE_RIGHT";
static const std::string kMoveUpKey = "MOVE_UP";
static const std::string kMoveDownKey = "MOVE_DOWN";
static const std::string kAttackKeyBind = "ATTACK";
static const std::string kHarakiriKeyBind = "HARAKIRI";
static const std::string kTeleportKeyBind = "TELEPORT";

} // namespace command_const

#endif // ANTIPATTERNS_GAMEPLAYCONSTANTS_H
