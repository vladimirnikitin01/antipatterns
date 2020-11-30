//
// Created by dmitry on 5/5/20.
//

#ifndef ANTIPATTERNS_PLAYERGUI_H
#define ANTIPATTERNS_PLAYERGUI_H

#include "../../src/Player.h"
#include "../Constants/Filenames.h"
#include "ProgressBar.h"

namespace gui {

class PlayerGUI {
public:
  PlayerGUI(std::shared_ptr<Player> player, sf::VideoMode video_mode,
            const std::string &file_name);

  void Update(float time_elapsed);

  void Render(sf::RenderTarget &target);

private:
  void InitFont();

  void InitHPBar(const std::map<std::string, Json::Node> &settings);

  void InitManaBar(const std::map<std::string, Json::Node> &settings);

  void InitExpBar(const std::map<std::string, Json::Node> &settings);

  void InitLevelBar(const std::map<std::string, Json::Node> &settings);

  std::shared_ptr<Player> player_;
  std::unique_ptr<gui::ProgressBar> hp_bar_;
  std::unique_ptr<gui::ProgressBar> mana_bar_;
  std::unique_ptr<gui::ProgressBar> exp_bar_;
  std::unique_ptr<gui::SimpleBar> level_bar_;

  sf::Font gui_font_;
  sf::VideoMode video_mode_;
};
} // namespace gui

#endif // ANTIPATTERNS_PLAYERGUI_H
