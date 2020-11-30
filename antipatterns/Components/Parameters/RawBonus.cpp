//
// Created by dmitry on 5/5/20.
//

#include "RawBonus.h"


/// ----------------- RAW BONUS ----------------- ///

RawBonus::RawBonus(ATTRIBUTE_ID id, Stats stats, LevelChange level_change) : BaseAttribute(stats, level_change) {
    id_ = id;
}


//actually only player has raw bonuses
std::shared_ptr<BaseAttribute> RawBonus::Clone() const {
    std::shared_ptr<RawBonus> copy = std::make_shared<RawBonus>(id_, stats_, level_change_);
    for (const auto &raw_bonus : raw_bonuses_) {
        copy->AddRawBonus(raw_bonus->Clone());
    }
    for (const auto &effect : effects_) {
        effect->AddEffect(effect->Clone());
    }
    copy->current_value_ = current_value_;
    copy->updated_ = updated_;
    copy->to_remove_ = to_remove_;
    return copy;
}

bool RawBonus::ToRemove() const {
    return to_remove_;
}

void RawBonus::SetAttributeId(ATTRIBUTE_ID id) {
    id_ = id;
}

void RawBonus::SetToRemove(bool value) {
    to_remove_ = value;
}