//
// Created by dmitry on 5/5/20.
//

#include "ExpComponent.h"

ExpComponent::ExpComponent(int level) : level_(level) {}

int ExpComponent::GainExp(int exp_gained) {
  int temp_exp = exp_gained + current_xp_;
  int temp_level{level_};
  while (temp_level < stat_const::kMaxLevel &&
         temp_exp < stat_const::kExpForNextLevel[temp_level]) {
    temp_exp -= stat_const::kExpForNextLevel[temp_level];
    temp_level++;
  }
  int result = temp_level - level_;
  current_xp_ = temp_exp;
  level_ = temp_level;
  return result;
}

void ExpComponent::ChangeLevel(int delta_level) {
  if (delta_level + level_ >= 1) {
    level_ += delta_level;
    current_xp_ = 0;
  }
}

int ExpComponent::GetCurrentExp() const { return current_xp_; }

int ExpComponent::GetLevel() const { return level_; }
