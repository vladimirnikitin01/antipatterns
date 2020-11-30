//
// Created by dmitry on 4/21/20.
//

#include "Attribute.h"



/// ---------------- ATTRIBUTE ---------------- ///

Attribute::Attribute(Stats stats, LevelChange level_change, StatsFunc calc_func)
        : BaseAttribute(stats, level_change),
          calc_func_(std::move(calc_func)) {
}

void Attribute::AddDependantAttribute(ATTRIBUTE_ID id, std::shared_ptr<BaseAttribute> attribute) {
    base_attributes_[id] = std::move(attribute);
}


void Attribute::Update(float time_elapsed) {
    if (updated_) {
        return;
    }
    updated_ = true;
    current_value_ = stats_.base_value;
    ApplyBaseAttributes(base_attributes_, current_value_);
    UpdateBonuses(time_elapsed, raw_bonuses_);
    UpdateBonuses(time_elapsed, effects_);
}

void Attribute::ApplyBaseAttributes(const BaseStats &base_attributes, double &base_value) {
    base_value += calc_func_(base_attributes);
}

std::shared_ptr<BaseAttribute> Attribute::Clone() const {
    std::shared_ptr<Attribute> copy = std::make_shared<Attribute>(stats_, level_change_, calc_func_);
    for (const auto &raw_bonus : raw_bonuses_) {
        copy->AddRawBonus(raw_bonus->Clone());
    }
    for (const auto &effect : effects_) {
        effect->AddEffect(effect->Clone());
    }
    copy->current_value_ = current_value_;
    copy->updated_ = updated_;
    copy->calc_func_ = calc_func_;
    return copy;
}



/// ------------ ATTRIBUTE VALUE ------------ ///

AttributeValue::AttributeValue(std::shared_ptr<BaseAttribute> max_value, double relative_value) : max_value_(
        std::move(max_value)), relative_value_(relative_value), BaseAttribute(Stats(), LevelChange()) {

}

std::shared_ptr<BaseAttribute> AttributeValue::Clone() const {
    return std::make_shared<AttributeValue>(nullptr, relative_value_);
}

void AttributeValue::AddDependantAttribute(ATTRIBUTE_ID id, std::shared_ptr<BaseAttribute> attribute) {
    max_value_ = std::move(attribute);
}

double AttributeValue::GetBaseValue() const {
    return max_value_->GetBaseValue();
}

double AttributeValue::GetCurrentValue() const {
    return max_value_->GetCurrentValue() * relative_value_;
}

double AttributeValue::GetMultiplier() const {
    return max_value_->GetMultiplier();
}

void AttributeValue::UpdateLevel(int level_change) {
    relative_value_ = 1;
}

void AttributeValue::SetCurrentValue(double value) {
    relative_value_ = value / max_value_->GetCurrentValue();
}

//responsible for its own value
void AttributeValue::Update(float time_elapsed) {
    if (updated_) {
        return;
    }
    updated_ = true;
    UpdateBonuses(time_elapsed, raw_bonuses_);
    UpdateBonuses(time_elapsed, effects_);
    if (relative_value_ < 0) {
        relative_value_ = 0;
    }
    if (relative_value_ > 1) {
        relative_value_ = 1;
    }
}


void AttributeValue::UpdateBonuses(float time_elapsed, std::list<std::shared_ptr<BaseAttribute>> &bonuses) {
    auto b_it = bonuses.begin();
    double bonus_value{0}, bonus_multiplier{0};
    while (b_it != bonuses.end()) {
        //порядок - подумать!!!!
      if ((*b_it)->IsReady()) {
        bonus_value += (*b_it)->GetCurrentValue();
        bonus_multiplier += (*b_it)->GetMultiplier();
      }
      (*b_it)->Update(time_elapsed);
      if ((*b_it)->ToRemove()) {
        b_it = bonuses.erase(b_it);
        continue;
      }
      ++b_it;
    }
    relative_value_ += bonus_value / max_value_->GetCurrentValue();
    relative_value_ *= (1.0 + bonus_multiplier);
}



