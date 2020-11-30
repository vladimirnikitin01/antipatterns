//
// Created by dmitry on 5/17/20.
//

#ifndef ANTIPATTERNS_MESSAGE_H
#define ANTIPATTERNS_MESSAGE_H

#include "../../Components/Parameters/Skill.h"
#include <unordered_map>

namespace message {
class Node : std::variant<Skill *> {
public:
  using variant::variant;

  const auto & AsSkillData() const {
    return std::get<Skill *>(*this);
  }
};

class Message {
public:
  explicit Message(Node data);

  [[nodiscard]] const Node &GetData() const;

private:
  Node data_;
};
} // namespace message

#endif // ANTIPATTERNS_MESSAGE_H
