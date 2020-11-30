//
// Created by dmitry on 5/1/20.
//

#ifndef ANTIPATTERNS_UTILITY_H
#define ANTIPATTERNS_UTILITY_H

#include "../Components/PhysicsComponent.h"
#include "../src/Tile.h"
#include "Constants/GamePlayConstants.h"
#include "RandomDevice.h"
#include <SFML/Audio.hpp>
#include <iostream>
#include <memory>
#include <type_traits>

namespace utility {

sf::FloatRect CreateSideRect(sf::Vector2f start, sf::Vector2f end);

float GetDistance(sf::Vector2f lhs, sf::Vector2f rhs);

sf::Vector2f GetDirection(MovementState state);

std::string GetCapsDirection(MovementState state);

float GetSpecialDistance(sf::Vector2f from, const sf::RectangleShape &shape);

float ScalarProduct(sf::Vector2f lhs, sf::Vector2f rhs);

float ScalarProduct(sf::Vector2f lhs, sf::Vector2f rhs_start,
                    sf::Vector2f rhs_end);

sf::Vector2f GenerateRandomDirection();

} // namespace utility

#endif // ANTIPATTERNS_UTILITY_H
