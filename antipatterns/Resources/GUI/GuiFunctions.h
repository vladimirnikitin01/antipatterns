//
// Created by dmitry on 5/5/20.
//

#ifndef ANTIPATTERNS_GUIFUNCTIONS_H
#define ANTIPATTERNS_GUIFUNCTIONS_H

#include "../Constants/Constants.h"
#include "../Json.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/VideoMode.hpp>

namespace gui {
// relative position in percents -> screen position in pixels
float ToPixelsX(float rel_value, sf::VideoMode &video_mode);

float ToPixelsY(float rel_value, sf::VideoMode &video_mode);

// finds ch.size for screen relating from 800 * 600
unsigned int CalcCharacterSize(sf::VideoMode &video_mode,
                               unsigned int normal_size);

sf::Color ParseColor(const std::vector<Json::Node> &color_json);
} // namespace gui

#endif // ANTIPATTERNS_GUIFUNCTIONS_H
