//
// Created by dmitry on 5/5/20.
//

#include "BaseAttribute.h"

BaseAttribute::BaseAttribute(Stats stats, LevelChange level_change) : stats_(stats), level_change_(level_change) {
    current_value_ = stats_.base_value;
}

std::shared_ptr<BaseAttribute> BaseAttribute::Clone() const {
    std::shared_ptr<BaseAttribute> copy = std::make_shared<BaseAttribute>(stats_, level_change_);
    for (const auto &raw_bonus : raw_bonuses_) {
        copy->AddRawBonus(raw_bonus->Clone());
    }
    for (const auto &effect : effects_) {
        effect->AddEffect(effect->Clone());
    }
    copy->current_value_ = current_value_;
    copy->updated_ = updated_;
    return copy;
}

void BaseAttribute::AddRawBonus(std::shared_ptr<BaseAttribute> raw_bonus) {
    raw_bonuses_.push_back(std::move(raw_bonus));
}

void BaseAttribute::AddEffect(std::shared_ptr<BaseAttribute> effect) {
    effects_.push_back(std::move(effect));
}

void BaseAttribute::AddDependantAttribute(ATTRIBUTE_ID id, std::shared_ptr<BaseAttribute> attribute) {

}

double BaseAttribute::GetBaseValue() const {
    return stats_.base_value;
}

double BaseAttribute::GetCurrentValue() const {
    return current_value_;
}

double BaseAttribute::GetMultiplier() const {
    return stats_.multiplier;
}

bool BaseAttribute::IsReady() const {
    return true;
}

bool BaseAttribute::ToRemove() const {
    return false;
}

void BaseAttribute::Update(float time_elapsed) {
    if (updated_) {
        return;
    }
    updated_ = true;
    current_value_ = stats_.base_value;
    //обновляем все постоянные бонусы
    UpdateBonuses(time_elapsed, raw_bonuses_);
    //обновляем все эффекты
    UpdateBonuses(time_elapsed, effects_);
}

void BaseAttribute::ResetUpdate() {
    updated_ = false;
    for (auto& raw_bonus : raw_bonuses_) {
        raw_bonus->ResetUpdate();
    }
    for (auto& effect : effects_) {
        effect->ResetUpdate();
    }
}

void BaseAttribute::UpdateLevel(int level_change) {
    current_value_ = stats_.base_value;
    for (int i = 0; i < level_change; ++i) {
        stats_.base_value += level_change_.delta_value;
        stats_.base_value *= (1.0 + level_change_.multiplier);
    }
    Update(0);
}

void BaseAttribute::SetCurrentValue(double value) {
    current_value_ = value;
}

void BaseAttribute::UpdateBonuses(float time_elapsed, std::list<std::shared_ptr<BaseAttribute>> &bonuses) {
    auto b_it = bonuses.begin();
    double bonus_value{0}, bonus_multiplier{0};
    while (b_it != bonuses.end()) {
        (*b_it)->Update(time_elapsed);
        if ((*b_it)->ToRemove()) {
            b_it = bonuses.erase(b_it);
            continue;
        }
        if ((*b_it)->IsReady()) {
            bonus_value += (*b_it)->GetCurrentValue();
            bonus_multiplier += (*b_it)->GetMultiplier();
        }
        ++b_it;
    }
    current_value_ += bonus_value;
    current_value_ *= (1.0 + bonus_multiplier);
}
