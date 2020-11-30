//
// Created by dmitry on 5/5/20.
//

#ifndef ANTIPATTERNS_BASEATTRIBUTE_H
#define ANTIPATTERNS_BASEATTRIBUTE_H

#include <cmath>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <vector>

enum class ATTRIBUTE_ID {
  VIGOR = 0,
  STRENGTH,
  DEXTERITY,
  INTELLIGENCE,
  FAITH,
  LUCK,
  MAX_HP,
  MAX_MANA,
  PHYS_ARMOR,
  MAG_ARMOR,
  ATTACK_SPEED,
  BASE_DAMAGE,
  CRIT_CHANCE,
  CURR_HP,
  CURR_MANA
};

enum class EFFECT_TYPE { STATS = 0, POISON, PHYSICAL_DAMAGE, MAGICAL_DAMAGE };

struct Stats {
  double base_value;
  double multiplier;

  Stats() = default;
  Stats(double base_value, double multiplier)
      : base_value(base_value), multiplier(multiplier) {}
};

struct LevelChange {
  double delta_value;
  double multiplier;

  LevelChange() = default;
  LevelChange(double delta_value, double multiplier)
      : delta_value(delta_value), multiplier(multiplier) {}
};

class BaseAttribute {
public:
  // BaseAttribute() = default;

  explicit BaseAttribute(Stats stats, LevelChange level_change);

  BaseAttribute(const BaseAttribute &other) = delete;
  BaseAttribute &operator=(const BaseAttribute &other) = delete;

  virtual std::shared_ptr<BaseAttribute> Clone() const;

  void AddRawBonus(std::shared_ptr<BaseAttribute> raw_bonus);

  void AddEffect(std::shared_ptr<BaseAttribute> effect);

  virtual void AddDependantAttribute(ATTRIBUTE_ID id,
                                     std::shared_ptr<BaseAttribute> attribute);

  virtual double GetBaseValue() const;

  virtual double GetCurrentValue() const;

  virtual double GetMultiplier() const;

  virtual bool IsReady() const;

  virtual bool ToRemove() const;

  virtual void Update(float time_elapsed);

  void ResetUpdate();

  virtual void UpdateLevel(int level_change);

  virtual void SetCurrentValue(double value);

protected:
  virtual void
  UpdateBonuses(float time_elapsed,
                std::list<std::shared_ptr<BaseAttribute>> &bonuses);

  std::list<std::shared_ptr<BaseAttribute>> raw_bonuses_{};
  std::list<std::shared_ptr<BaseAttribute>> effects_{};
  Stats stats_;
  LevelChange level_change_;
  double current_value_;
  bool updated_{false};

private:
};

#endif // ANTIPATTERNS_BASEATTRIBUTE_H
