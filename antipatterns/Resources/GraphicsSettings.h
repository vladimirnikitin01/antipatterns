//
// Created by dmitry on 3/30/20.
//

#ifndef ANTIPATTERNS_GRAPHICSSETTINGS_H
#define ANTIPATTERNS_GRAPHICSSETTINGS_H

#include "Json.h"
#include "random"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class GraphicsSettings {
public:
  std::string game_title_;
  sf::VideoMode resolution_;
  std::vector<sf::VideoMode> video_modes_;
  sf::ContextSettings settings_;
  unsigned int framerate_limit_;
  bool fullscreen_;
  bool v_sync_enabled_;

  GraphicsSettings();

  bool LoadFromFile(const std::string &file_name);
};

#endif // ANTIPATTERNS_GRAPHICSSETTINGS_H
