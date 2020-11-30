//
// Created by dmitry on 4/21/20.
//

#ifndef ANTIPATTERNS_ATTRIBUTE_H
#define ANTIPATTERNS_ATTRIBUTE_H

#include "RawBonus.h"


class Attribute : public BaseAttribute {
public:
    using BaseStats = std::unordered_map<ATTRIBUTE_ID, std::shared_ptr<BaseAttribute>>;
    using StatsFunc = std::function<double(const std::unordered_map<ATTRIBUTE_ID, std::shared_ptr<BaseAttribute>>&)>;

    Attribute(Stats stats, LevelChange level_change, StatsFunc calc_func);

    std::shared_ptr<BaseAttribute> Clone() const override;

    void AddDependantAttribute(ATTRIBUTE_ID id, std::shared_ptr<BaseAttribute> attribute) override;

    void Update(float time_elapsed) override;
protected:
    void ApplyBaseAttributes(const BaseStats& base_attributes, double& base_value);

    BaseStats base_attributes_;
    StatsFunc calc_func_;
private:

};


class AttributeValue : public BaseAttribute {
public:
    AttributeValue() = default;

    explicit AttributeValue(std::shared_ptr<BaseAttribute> max_value, double relative_value = 1);

    std::shared_ptr<BaseAttribute> Clone() const override;

    void AddDependantAttribute(ATTRIBUTE_ID id, std::shared_ptr<BaseAttribute> attribute) override;

    double GetBaseValue() const override;

    double GetCurrentValue() const override;

    double GetMultiplier() const override;

    void Update(float time_elapsed) override;

    void UpdateLevel(int level_change) override;

    void SetCurrentValue(double value) override;

protected:
    void UpdateBonuses(float time_elapsed, std::list<std::shared_ptr<BaseAttribute>>& bonuses) override;
    std::shared_ptr<BaseAttribute> max_value_;
    double relative_value_{1};
};

#endif //ANTIPATTERNS_ATTRIBUTE_H
