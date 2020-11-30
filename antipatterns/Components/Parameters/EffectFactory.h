//
// Created by dmitry on 5/16/20.
//

#ifndef ANTIPATTERNS_EFFECTFACTORY_H
#define ANTIPATTERNS_EFFECTFACTORY_H

#include "../../Resources/Constants/GamePlayConstants.h"
#include "Effect.h"

class EffectFactory {
public:
  using BaseStats = std::vector<std::shared_ptr<BaseAttribute>>;
  using EffectFunc = std::function<void(Effect &, const BaseStats &)>;
  using EffectFuncs = std::unordered_map<std::string, EffectFunc>;

  EffectFactory() = default;

  std::shared_ptr<Effect>
  CreateEffect(const std::map<std::string, Json::Node> &settings);

private:
  static const std::unordered_map<EFFECT_TYPE,
                                  std::unordered_map<std::string, EffectFunc>>
      effect_type_to_func;
};

#endif // ANTIPATTERNS_EFFECTFACTORY_H
