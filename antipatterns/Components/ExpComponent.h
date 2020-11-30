//
// Created by dmitry on 5/5/20.
//

#ifndef ANTIPATTERNS_EXPCOMPONENT_H
#define ANTIPATTERNS_EXPCOMPONENT_H

#include "../Resources/Constants/GamePlayConstants.h"

class ExpComponent {
public:
  explicit ExpComponent(int level = 1);

  int GainExp(int exp_gained);

  void ChangeLevel(int delta_level);

  int GetCurrentExp() const;

  int GetLevel() const;

private:
  int current_xp_{0};
  int level_;
};

#endif // ANTIPATTERNS_EXPCOMPONENT_H
