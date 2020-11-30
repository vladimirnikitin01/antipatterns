//
// Created by dmitry on 4/29/20.
//

#ifndef ANTIPATTERNS_CONSTANTS_H
#define ANTIPATTERNS_CONSTANTS_H

#include <SFML/Window/Keyboard.hpp>
#include <limits>
#include <string>

const double kDInfinity = std::numeric_limits<double>::infinity();
const float kFInfinity = std::numeric_limits<float>::infinity();

const float kNormalHeight = 600;
const float kNormalWidth = 800;

const float kTimeNormalize = 100;
const sf::Keyboard::Key kUnknownKey = sf::Keyboard::Unknown;

static const std::string kPlayText = "PLAY";
static const std::string kQuitText = "QUIT";
static const std::string kPauseText = "PAUSE";
static const std::string kCloseText = "CLOSE";
static const std::string kPlayerText = "PLAYER";

#endif // ANTIPATTERNS_CONSTANTS_H
