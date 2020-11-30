//

//
// Created by dmitry on 4/2/20.

#include "AttributeComponent.h"
#include "Parameters/EffectFactory.h"

const std::unordered_map<ATTRIBUTE_ID, std::vector<ATTRIBUTE_ID>>
    AttributeComponent::dependence_{
        {ATTRIBUTE_ID::MAX_HP, {ATTRIBUTE_ID::VIGOR}},
        {ATTRIBUTE_ID::MAX_MANA,
         {ATTRIBUTE_ID::INTELLIGENCE, ATTRIBUTE_ID::FAITH}},
        {ATTRIBUTE_ID::PHYS_ARMOR,
         {ATTRIBUTE_ID::VIGOR, ATTRIBUTE_ID::STRENGTH,
          ATTRIBUTE_ID::DEXTERITY}},
        {ATTRIBUTE_ID::MAG_ARMOR,
         {ATTRIBUTE_ID::FAITH, ATTRIBUTE_ID::VIGOR,
          ATTRIBUTE_ID::INTELLIGENCE}},
        {ATTRIBUTE_ID::ATTACK_SPEED,
         {ATTRIBUTE_ID::DEXTERITY, ATTRIBUTE_ID::STRENGTH,
          ATTRIBUTE_ID::INTELLIGENCE}},
        {ATTRIBUTE_ID::BASE_DAMAGE,
         {ATTRIBUTE_ID::STRENGTH, ATTRIBUTE_ID::DEXTERITY}},
        {ATTRIBUTE_ID::CRIT_CHANCE,
         {ATTRIBUTE_ID::LUCK, ATTRIBUTE_ID::DEXTERITY}},
        {ATTRIBUTE_ID::CURR_HP, {ATTRIBUTE_ID::MAX_HP}},
        {ATTRIBUTE_ID::CURR_MANA, {ATTRIBUTE_ID::MAX_MANA}}};

const std::unordered_map<ATTRIBUTE_ID, std::pair<double, double>>
    AttributeComponent::id_boundaries_{
        {ATTRIBUTE_ID::VIGOR, {0, kDInfinity}},
        {ATTRIBUTE_ID::STRENGTH, {0, kDInfinity}},
        {ATTRIBUTE_ID::DEXTERITY, {0, kDInfinity}},
        {ATTRIBUTE_ID::INTELLIGENCE, {0, kDInfinity}},
        {ATTRIBUTE_ID::FAITH, {0, kDInfinity}},
        {ATTRIBUTE_ID::LUCK, {0, kDInfinity}},
        {ATTRIBUTE_ID::MAX_HP, {1, kDInfinity}},
        {ATTRIBUTE_ID::MAX_MANA, {1, kDInfinity}},
        {ATTRIBUTE_ID::PHYS_ARMOR, {-kDInfinity, 1}},
        {ATTRIBUTE_ID::MAG_ARMOR, {-kDInfinity, 1}},
        {ATTRIBUTE_ID::ATTACK_SPEED, {0, kDInfinity}},
        {ATTRIBUTE_ID::BASE_DAMAGE, {0, kDInfinity}},
        {ATTRIBUTE_ID::CRIT_CHANCE, {0, 1}}};

const std::unordered_map<
    ATTRIBUTE_ID,
    std::function<double(const std::unordered_map<
                         ATTRIBUTE_ID, std::shared_ptr<BaseAttribute>> &)>>
    AttributeComponent::id_to_functions_{
        {ATTRIBUTE_ID::MAX_HP, stats_formula::MaxHPFunction},
        {ATTRIBUTE_ID::MAX_MANA, stats_formula::MaxManaFunction},
        {ATTRIBUTE_ID::PHYS_ARMOR, stats_formula::PhysArmorFunction},
        {ATTRIBUTE_ID::MAG_ARMOR, stats_formula::MagArmorFunction},
        {ATTRIBUTE_ID::ATTACK_SPEED, stats_formula::AttackSpeedFunction},
        {ATTRIBUTE_ID::BASE_DAMAGE, stats_formula::BaseDamageFunction},
        {ATTRIBUTE_ID::CRIT_CHANCE, stats_formula::CritChanceFunction},
    };

AttributeComponent::AttributeComponent(
    const std::map<std::string, Json::Node> &settings) {
  attributes_.resize(stat_const::names_to_id.size());
  LoadFromMap(settings);
  this->Update(0);
}

AttributeComponent::AttributeComponent(const AttributeComponent &other) {
  *this = other;
}

AttributeComponent &
AttributeComponent::operator=(const AttributeComponent &other) {
  if (&other == this) {
    return *this;
  }
  attributes_ = std::vector<std::shared_ptr<BaseAttribute>>();
  for (const auto &attribute : other.attributes_) {
    attributes_.push_back(attribute->Clone());
  }
  ResetAttributesDependence();
  this->Update(0);
  return *this;
}

void AttributeComponent::Update(float time_elapsed) {
  for (auto &attribute : attributes_) {
    attribute->Update(time_elapsed);
  }
  CheckBoundaries();
  for (auto &attribute : attributes_) {
    attribute->ResetUpdate();
  }
}

void AttributeComponent::AddEffect(std::shared_ptr<Effect> effect) {
  ATTRIBUTE_ID id_to_attach = effect->GetAttributeid();
  attributes_.at(static_cast<int>(id_to_attach))->AddEffect(std::move(effect));
}

double AttributeComponent::GetAttributeValue(ATTRIBUTE_ID id) {
  return attributes_.at(static_cast<size_t>(id))->GetCurrentValue();
}

const std::vector<std::shared_ptr<BaseAttribute>> &
AttributeComponent::GetAttributes() const {
  return attributes_;
}

void AttributeComponent::ResetHpMana() {
  attributes_.at(static_cast<int>(ATTRIBUTE_ID::CURR_HP))
      ->SetCurrentValue(
          attributes_.at(static_cast<int>(ATTRIBUTE_ID::MAX_HP))
              ->GetCurrentValue());
  attributes_.at(static_cast<int>(ATTRIBUTE_ID::CURR_MANA))
      ->SetCurrentValue(
          attributes_.at(static_cast<int>(ATTRIBUTE_ID::MAX_MANA))
              ->GetCurrentValue());
}

void AttributeComponent::UpdateLevel(int level_change) {
  for (auto &attr : attributes_) {
    attr->UpdateLevel(level_change);
  }
}


void AttributeComponent::LoadFromMap(
    const std::map<std::string, Json::Node> &settings) {
  LoadBaseAttributes(settings.at("base_attributes").AsMap());
  LoadDependantAttributes(settings.at("dependant_attributes").AsMap());
  LoadChangingAttributes(settings.at("changing_attributes").AsMap());
}

void AttributeComponent::ResetAttributesDependence() {
  for (size_t curr_index = 0; curr_index < attributes_.size(); ++curr_index) {
    auto curr_id = static_cast<ATTRIBUTE_ID>(curr_index);
    if (dependence_.count(curr_id) != 0) {
      for (ATTRIBUTE_ID id : dependence_.at(curr_id)) {
        attributes_[curr_index]->AddDependantAttribute(
            id, attributes_.at(static_cast<size_t>(id)));
      }
    }
  }
}

void AttributeComponent::CheckBoundaries() {
  for (size_t i = 0; i < id_boundaries_.size(); ++i) {
    auto curr_id = static_cast<ATTRIBUTE_ID>(i);
    if (attributes_[i]->GetCurrentValue() < id_boundaries_.at(curr_id).first) {
      attributes_[i]->SetCurrentValue(id_boundaries_.at(curr_id).first);
    } else if (attributes_[i]->GetCurrentValue() >
               id_boundaries_.at(curr_id).second) {
      attributes_[i]->SetCurrentValue(id_boundaries_.at(curr_id).second);
    }
  }
}

void AttributeComponent::LoadBaseAttributes(
    const std::map<std::string, Json::Node> &settings) {
  Stats temp_stats{};
  LevelChange temp_level_change{};
  for (const auto &[attr_name, attr_settings] : settings) {
    GetBaseStats(temp_stats, temp_level_change, attr_settings.AsMap());
    auto curr_id = stat_const::names_to_id.at(attr_name);
    auto curr_index = static_cast<size_t>(curr_id);
    attributes_.at(curr_index) =
        std::make_shared<BaseAttribute>(temp_stats, temp_level_change);
  }
}

void AttributeComponent::LoadDependantAttributes(
    const std::map<std::string, Json::Node> &settings) {
  Stats temp_stats{};
  LevelChange level_change{};
  for (const auto &[attr_name, attr_settings] : settings) {
    GetBaseStats(temp_stats, level_change, attr_settings.AsMap());
    auto curr_id = stat_const::names_to_id.at(attr_name);
    auto curr_index = static_cast<size_t>(curr_id);
    std::shared_ptr<Attribute> curr_attr = std::make_shared<Attribute>(
        temp_stats, level_change,
        id_to_functions_.at(static_cast<ATTRIBUTE_ID>(curr_index)));
    for (const ATTRIBUTE_ID &dependent_id : dependence_.at(curr_id)) {
      curr_attr->AddDependantAttribute(
          dependent_id, attributes_.at(static_cast<int>(dependent_id)));
    }
    attributes_.at(curr_index) = std::move(curr_attr);
  }
}

void AttributeComponent::LoadChangingAttributes(
    const std::map<std::string, Json::Node> &settings) {
  for (const auto &[attr_name, attr_settings] : settings) {
    auto curr_id = stat_const::names_to_id.at(attr_name);
    auto curr_index = static_cast<size_t>(curr_id);
    //вектор dependence.at(curr_id) гарантированно содержит всего 1 элемент
    auto max_value_index = static_cast<size_t>(dependence_.at(curr_id).front());
    attributes_.at(curr_index) =
        std::make_shared<AttributeValue>(attributes_.at(max_value_index));
  }
}
void AttributeComponent::GetBaseStats(
    Stats &stats, LevelChange &level_change,
    const std::map<std::string, Json::Node> &settings) {
  stats.base_value = settings.at("stats").AsMap().at("base_value").AsDouble();
  stats.multiplier = settings.at("stats").AsMap().at("multiplier").AsDouble();
  level_change.delta_value =
      settings.at("level_change").AsMap().at("delta_value").AsDouble();
  level_change.multiplier =
      settings.at("level_change").AsMap().at("multiplier").AsDouble();
}

