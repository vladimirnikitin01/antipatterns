//
// Created by dmitry on 3/28/20.
//

#include "GameState.h"

#include <utility>

const std::unordered_map<std::string, ROOM_ID> GameState::rooms_names_to_ids = {
    {"init_room", ROOM_ID::INIT_ROOM}, {"second_room", ROOM_ID::SECOND_ROOM}};

GameState::GameState(
    std::shared_ptr<sf::RenderWindow> window,
    std::shared_ptr<std::unordered_map<std::string, int>> supported_keys,
    std::shared_ptr<std::stack<std::shared_ptr<State>>> state_stack)
    : State(std::move(window), std::move(supported_keys),
            std::move(state_stack)) {
  GameState::InitKeybindings();
  GameState::InitPlayer();
  GameState::InitRooms();
  GameState::InitPlayerGui();
  GameState::InitPauseMenu();
  GameState::InitInputHandler();
}

GameState::~GameState() {}

/* overrided functions */

void GameState::Update(const float time_elapsed) {
  /* if not paused, update game */
  UpdateMousePositions();
  CheckEnd();
  if (!_paused) {
    UpdateInput(time_elapsed);
    player_gui_->Update(time_elapsed);
    for (auto &[id, room_ptr] : rooms_) {
      room_ptr->Update(time_elapsed);
    }
    const Exit *active_exit = rooms_.at(current_room_id)->GetActiveExit();
    if (active_exit != nullptr) {
      ChangeRoom(current_room_id, active_exit->GetRoomIdTo(),
                 active_exit->GetPlayerPos());
    }

  } else {
    pause_menu_->Update(mouse_positions_.view);
    if (pause_menu_->IsButtonActive(std::string(kPlayText))) {
      Unpause();
    }
    if (pause_menu_->IsButtonActive(std::string(kQuitText))) {
      End();
    }
  }
}

void GameState::UpdateInput(const float time_elapsed) {

  if (sf::Keyboard::isKeyPressed(
          static_cast<sf::Keyboard::Key>(keybindings_->at(kPauseText)))) {
    Pause();
  }
  if (sf::Keyboard::isKeyPressed(
          static_cast<sf::Keyboard::Key>(keybindings_->at(kCloseText)))) {
    End();
  }

  input_handler_->UpdateInput(time_elapsed);
  std::shared_ptr<GameCommand> last_command_ = input_handler_->GetLastCommand();
  if (last_command_ != nullptr) {
    last_command_->Execute(time_elapsed);
    if (last_command_->IsDone()) {
      rooms_.at(current_room_id)->ReceiveMessage(last_command_->GetMessage());
      input_handler_->Reset();
    }
  }
}

void GameState::Render(std::shared_ptr<sf::RenderTarget> target) {
  rooms_.at(current_room_id)->Render(*target);
  player_gui_->Render(*target);
  if (_paused) {
    pause_menu_->Render(*target);
  }
}

void GameState::ChangeRoom(ROOM_ID old_room, ROOM_ID new_room,
                           sf::Vector2f new_pos) {
  rooms_.at(new_room)->SetPlayer(player_);
  current_room_id = new_room;
  player_->SetPosition(new_pos);
  rooms_.at(old_room)->SetPlayer(nullptr);
}

/* initializers */

void GameState::InitKeybindings() {
  std::ifstream in(file::kGameStateKeybindingsFile);
  if (in.is_open()) {
    std::string key_str;
    std::string key_bind;
    while (in >> key_str >> key_bind) {
      keybindings_->try_emplace(key_str, supported_keys_->at(key_bind));
    }
  }
}

void GameState::InitPlayer() {
  player_ = std::make_shared<Player>(
      sf::Vector2f(room_const::kInitPlayerX, room_const::kInitPlayerY),
      UniqueDatabase::Instance().GetData().textures.at(kPlayerText),
      file::kPlayerSettingsFile);
}

void GameState::InitPauseMenu() {
  pause_menu_ = std::make_shared<PauseMenu>(window_);
}

void GameState::InitPlayerGui() {
  player_gui_ = std::make_shared<gui::PlayerGUI>(
      player_, sf::VideoMode(window_->getSize().x, window_->getSize().y),
      file::kPlayerGuiFile);
}

void GameState::InitRooms() {
  const std::map<std::string, Json::Node> rooms_settings =
      Json::Load(file::kRoomSettingsFile).GetRoot().AsMap();
  for (const auto &[room_name, room_settings_json] : rooms_settings) {
    const auto &room_settings = room_settings_json.AsMap();
    ROOM_ID room_id = rooms_names_to_ids.at(room_name);
    rooms_[room_id] =
        std::make_unique<Room>(room_settings.at("file_name").AsString());
    std::string exits_file_name = room_settings.at("exits").AsString();
    const std::vector<Json::Node> room_exits_settings =
        Json::Load(exits_file_name).GetRoot().AsArray();
    for (const auto &exit_node : room_exits_settings) {
      const auto &exit_settings_map = exit_node.AsMap();
      ExitType curr_type =
          static_cast<ExitType>(exit_settings_map.at("type").AsDouble());
      std::unique_ptr<Exit> curr_exit = std::make_unique<Exit>(
          *UniqueDatabase::Instance().GetData().exits.at(curr_type));
      curr_exit->LoadFromMap(exit_settings_map.at("settings").AsMap());
      rooms_.at(room_id)->AddExit(std::move(curr_exit));
    }
    current_room_id = ROOM_ID::INIT_ROOM;
    rooms_.at(current_room_id)->SetPlayer(player_);
  }
}

void GameState::InitInputHandler() {
  input_handler_ = std::make_unique<PlayerInputHandler>(player_, keybindings_);
}

void GameState::CheckEnd() {
  if (player_->IsDead()) {
    _to_quit = true;
  }
}
