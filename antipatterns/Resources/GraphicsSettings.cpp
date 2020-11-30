//
// Created by dmitry on 3/30/20.
//

#include "GraphicsSettings.h"

GraphicsSettings::GraphicsSettings() {
  game_title_ = "Not antipatterns actually";
  resolution_ = sf::VideoMode::getDesktopMode();
  framerate_limit_ = 60;
  fullscreen_ = false;
  v_sync_enabled_ = false;
  settings_.antialiasingLevel = 0;
  video_modes_ = sf::VideoMode::getFullscreenModes();
}

bool GraphicsSettings::LoadFromFile(const std::string &file_name) {
  std::ifstream in(file_name);
  bool flag = in.is_open();
  if (in.is_open()) {
    std::getline(in, game_title_);
    in >> resolution_.width >> resolution_.height;
    in >> framerate_limit_;
    in >> fullscreen_;
    in >> v_sync_enabled_;
    in >> settings_.antialiasingLevel;
  }
  in.close();
  return flag;
}
