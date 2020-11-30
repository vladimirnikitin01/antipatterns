//
// Created by dmitry on 5/5/20.
//

#ifndef ANTIPATTERNS_RAWBONUS_H
#define ANTIPATTERNS_RAWBONUS_H

#include "BaseAttribute.h"

class RawBonus : public BaseAttribute {
public:
    explicit RawBonus(ATTRIBUTE_ID id, Stats stats, LevelChange level_change);

    std::shared_ptr<BaseAttribute> Clone() const override;

    bool ToRemove() const override;

    void SetAttributeId(ATTRIBUTE_ID id);

    void SetToRemove(bool value);

private:
    bool to_remove_{false};
    ATTRIBUTE_ID id_;
};
#endif //ANTIPATTERNS_RAWBONUS_H
