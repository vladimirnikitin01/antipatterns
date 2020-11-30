//
// Created by dmitry on 5/5/20.
//

#include "PlayerGUI.h"

gui::PlayerGUI::PlayerGUI(std::shared_ptr<Player> player,
                          sf::VideoMode video_mode,
                          const std::string &file_name)
    : player_(std::move(player)), video_mode_(video_mode) {
  InitFont();
  const std::map<std::string, Json::Node> settings =
      Json::Load(file_name).GetRoot().AsMap();
  InitHPBar(settings.at("hp_bar").AsMap());
  InitManaBar(settings.at("mana_bar").AsMap());
  InitExpBar(settings.at("exp_bar").AsMap());
  InitLevelBar(settings.at("level_bar").AsMap());
}

void gui::PlayerGUI::Update(float time_elapsed) {
  double current_hp = player_->GetAttributeComponent()->GetAttributeValue(
      ATTRIBUTE_ID::CURR_HP);
  double max_hp =
      player_->GetAttributeComponent()->GetAttributeValue(ATTRIBUTE_ID::MAX_HP);
  hp_bar_->Update(current_hp, max_hp);
  double current_mana = player_->GetAttributeComponent()->GetAttributeValue(
      ATTRIBUTE_ID::CURR_MANA);
  double max_mana = player_->GetAttributeComponent()->GetAttributeValue(
      ATTRIBUTE_ID::MAX_MANA);
  mana_bar_->Update(current_mana, max_mana);
  int current_level = player_->GetExpComponent()->GetLevel();
  int current_exp = player_->GetExpComponent()->GetCurrentExp();
  int exp_for_next_level = stat_const::kExpForNextLevel[current_level];
  exp_bar_->Update(current_exp, exp_for_next_level);
  level_bar_->Update(current_level);
}

void gui::PlayerGUI::Render(sf::RenderTarget &target) {
  hp_bar_->Render(target);
  mana_bar_->Render(target);
  exp_bar_->Render(target);
  level_bar_->Render(target);
}

void gui::PlayerGUI::InitFont() { gui_font_.loadFromFile(file::kMainFontFile); }

void gui::PlayerGUI::InitHPBar(
    const std::map<std::string, Json::Node> &settings) {
  hp_bar_ = std::make_unique<ProgressBar>(video_mode_, settings, &gui_font_);
}

void gui::PlayerGUI::InitManaBar(
    const std::map<std::string, Json::Node> &settings) {
  mana_bar_ = std::make_unique<ProgressBar>(video_mode_, settings, &gui_font_);
}

void gui::PlayerGUI::InitExpBar(
    const std::map<std::string, Json::Node> &settings) {
  exp_bar_ = std::make_unique<ProgressBar>(video_mode_, settings, &gui_font_);
}

void gui::PlayerGUI::InitLevelBar(
    const std::map<std::string, Json::Node> &settings) {
  level_bar_ = std::make_unique<SimpleBar>(video_mode_, settings);
  level_bar_->SetFont(gui_font_);
}
