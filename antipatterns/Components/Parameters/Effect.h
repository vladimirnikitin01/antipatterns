//
// Created by dmitry on 4/21/20.
//

#ifndef ANTIPATTERNS_EFFECT_H
#define ANTIPATTERNS_EFFECT_H

#include "../../Resources/Constants/AttributeFormulas.h"
#include "../../Resources/Constants/GamePlayConstants.h"
#include "../../Resources/Json.h"
#include "Attribute.h"


class Effect : public BaseAttribute {
public:
  using BaseStats = std::vector<std::shared_ptr<BaseAttribute>>;
  using EffectFunc = std::function<void(Effect &, const BaseStats &)>;
  using EffectFuncs = std::unordered_map<std::string, EffectFunc>;

  Effect(ATTRIBUTE_ID id, Stats stats, LevelChange level_change,
         const EffectFuncs *effect_funcs = nullptr);

  std::shared_ptr<BaseAttribute> Clone() const override;

  ATTRIBUTE_ID GetAttributeid() const;

  void SetAttributeId(ATTRIBUTE_ID id);

  void SetMultiplier(double multiplier);

  virtual void LoadFromMap(const std::map<std::string, Json::Node> &settings);

  void UpdateAttributes(const std::string &update_key, const BaseStats &stats);
protected:

  const std::unordered_map<std::string, EffectFunc> *effect_funcs_;
  ATTRIBUTE_ID id_;
};

class OperatingEffect : public Effect {
public:
  OperatingEffect(ATTRIBUTE_ID id, Stats stats, LevelChange level_change,
                  const EffectFuncs *effect_funcs = nullptr);

  std::shared_ptr<BaseAttribute> Clone() const override;

  bool ToRemove() const override;

  void SetToRemove(bool value);

  bool IsReady() const override;

  void LoadFromMap(const std::map<std::string, Json::Node> &settings) override;

protected:
  mutable bool to_remove_{false};
};

class TimedEffect : public Effect {
public:
  TimedEffect(ATTRIBUTE_ID id, Stats stats, LevelChange level_change,
              const EffectFuncs *effect_funcs = nullptr);

  std::shared_ptr<BaseAttribute> Clone() const override;

  bool ToRemove() const override;

  void Update(float time_elapsed) override;

  void SetExpirationTime(float time_to_expire);

  void LoadFromMap(const std::map<std::string, Json::Node> &settings) override;

protected:
  float time_to_expire_{std::numeric_limits<float>::infinity()};

private:
};

class OverTimeEffect : public TimedEffect {
public:
  OverTimeEffect(ATTRIBUTE_ID id, Stats stats, LevelChange level_change,
                 const EffectFuncs *effect_funcs = nullptr);

  std::shared_ptr<BaseAttribute> Clone() const override;

  void Update(float time_elapsed) override;

  void SetTickTime(float tick_time);

  void LoadFromMap(const std::map<std::string, Json::Node> &settings) override;

protected:
  void SetTimeFromTick(float time_from_tick);

  float tick_time_{1.0};
  mutable float time_from_tick_{0};
};

class ProckingEffect : public OverTimeEffect {
public:
  ProckingEffect(ATTRIBUTE_ID id, Stats stats, LevelChange level_change,
                 const EffectFuncs *effect_funcs = nullptr);

  std::shared_ptr<BaseAttribute> Clone() const override;

  bool IsReady() const override;

  void LoadFromMap(const std::map<std::string, Json::Node> &settings) override;
};

#endif // ANTIPATTERNS_EFFECT_H
