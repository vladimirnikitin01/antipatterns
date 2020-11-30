//
// Created by dmitry on 4/2/20.
//

#ifndef ANTIPATTERNS_ATTRIBUTECOMPONENT_H
#define ANTIPATTERNS_ATTRIBUTECOMPONENT_H

#include "../Resources/Constants/AttributeFormulas.h"
#include "../Resources/Constants/Constants.h"
#include "ExpComponent.h"
#include "GraphicsComponent.h"
#include "Parameters/Attribute.h"
#include "Parameters/Effect.h"

class AttributeComponent {
public:
  explicit AttributeComponent(
      const std::map<std::string, Json::Node> &settings);

  AttributeComponent(const AttributeComponent &other);

  AttributeComponent &operator=(const AttributeComponent &other);

  ~AttributeComponent() = default;

  void Update(float time_elapsed);

  void AddEffect(std::shared_ptr<Effect> effect);

  double GetAttributeValue(ATTRIBUTE_ID id);

  [[nodiscard]] const std::vector<std::shared_ptr<BaseAttribute>> &
  GetAttributes() const;

  void ResetHpMana();

  void UpdateLevel(int level_change);

private:
  void LoadFromMap(const std::map<std::string, Json::Node> &settings);

  void ResetAttributesDependence();

  void CheckBoundaries();

  void LoadBaseAttributes(const std::map<std::string, Json::Node> &settings);

  void
  LoadDependantAttributes(const std::map<std::string, Json::Node> &settings);

  void
  LoadChangingAttributes(const std::map<std::string, Json::Node> &settings);

  void GetBaseStats(Stats &stats, LevelChange &level_change,
                    const std::map<std::string, Json::Node> &settings);

  std::vector<std::shared_ptr<BaseAttribute>> attributes_;

  static const std::unordered_map<ATTRIBUTE_ID, std::vector<ATTRIBUTE_ID>>
      dependence_;

  static const std::unordered_map<ATTRIBUTE_ID, std::pair<double, double>>
      id_boundaries_;
  using BaseStats =
      std::unordered_map<ATTRIBUTE_ID, std::shared_ptr<BaseAttribute>>;
  using StatsFunc = std::function<double(const BaseStats &)>;

  static const std::unordered_map<ATTRIBUTE_ID, StatsFunc> id_to_functions_;
};

#endif // ANTIPATTERNS_ATTRIBUTECOMPONENT_H
