//
// Created by dmitry on 5/17/20.
//

#include "Message.h"

#include <utility>

message::Message::Message(message::Node data) : data_(std::move(data)) {}
const message::Node &message::Message::GetData() const { return data_; }
