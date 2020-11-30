//
// Created by dmitry on 3/29/20.
//

#ifndef ANTIPATTERNS_BUTTONMANAGER_H
#define ANTIPATTERNS_BUTTONMANAGER_H

#include "../Constants/ButtonNames.h"
#include "../Constants/Filenames.h"
#include "../Json.h"
#include "Button.h"

namespace gui {
class ButtonManager {
public:
  ButtonManager(const std::string &params_file, const std::string &font_file);

  void LoadFont(const std::string &file_name);

  void InitButtons(const std::string &file_name);

  void Update(const sf::Vector2f &mouse_pos);

  void Render(sf::RenderTarget &target) const;

  const sf::Font &GetFont();

  bool IsActive(const std::string &button_name) const;

  std::shared_ptr<Button> operator[](const std::string &btn_name) const;

  std::unordered_map<std::string, std::shared_ptr<Button>>::iterator begin();

  std::unordered_map<std::string, std::shared_ptr<Button>>::iterator end();

private:
  std::unordered_map<std::string, std::shared_ptr<Button>> buttons_;
  sf::Font button_font_;
};
} // namespace gui

#endif // ANTIPATTERNS_BUTTONMANAGER_H
