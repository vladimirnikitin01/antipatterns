//
// Created by dmitry on 5/2/20.
//

#ifndef ANTIPATTERNS_FILENAMES_H
#define ANTIPATTERNS_FILENAMES_H

#include <string>

// Fonts
namespace file {

static const std::string kMainFontFile = "../Config/Lobster-Regular.ttf";
static const std::string kWindowSettingsFile = "../Config/window_init.txt";
static const std::string kSupportedKeysFile = "../Config/supported_keys.txt";

// files used in GameState.h
static const std::string kGameStateKeybindingsFile =
    "../Config/game_state_keybindings.txt";
static const std::string kPlayerGuiFile = "../Config/player_gui.json";
static const std::string kUniqueDataFile = "../Config/unique_data.json";
static const std::string kPlayerSettingsFile = "../Config/player_settings.json";
static const std::string kRoomSettingsFile = "../Config/rooms.json";

// files used in MainMenuState.h

static const std::string kMainMenuButtonsFile =
    "../Config/main_menu_buttons.json";
static const std::string kPauseMenuButtonsFile =
    "../Config/pause_menu_buttons.json";
static const std::string kMainMenuStateKeybindingsFile =
    "../Config/main_menu_state_keybindings.txt";
static const std::string kMainMenuBackgroundFile =
    "../Images/Backgrounds/main_menu.png";

} // namespace file

#endif // ANTIPATTERNS_FILENAMES_H
