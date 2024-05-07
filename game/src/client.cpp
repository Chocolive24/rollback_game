#include "client.h"

#include <imgui.h>
#include <string>

void Client::Init(int input_profile_id) noexcept {
  //network_manager_.RegisterClient(this);
  //network_manager_.RegisterOnlineGameManager(&online_game_manager_);
  //network_manager_.Connect();
 
  online_game_manager_.Init(input_profile_id);
  //online_game_manager_.RegisterNetworkInterface(&network_manager_);
 
  game_renderer_.Init();

  audio_manager_.Init();
  audio_manager_.PlayMusic(MusicType::kStartMenu);
}

void Client::Update() noexcept {
  const auto delta_time = raylib::GetFrameTime();
  fixed_timer_ += delta_time;
  time_since_last_fixed_update_ += delta_time;

  //network_manager_.Service();
  audio_manager_.Update();

  while (fixed_timer_ >= game_constants::kFixedDeltaTime) {
    if (state_ == ClientState::kInGame) {
      online_game_manager_.FixedUpdateCurrentFrame();
    }

    fixed_timer_ -= game_constants::kFixedDeltaTime;
    time_since_last_fixed_update_ = 0.f;
  }
}

void Client::Draw(const raylib::RenderTexture2D& render_texture, 
    raylib::Vector2 render_target_pos) noexcept {
  if (state_ != ClientState::kInGame) 
      return;
  game_renderer_.Draw(render_texture, render_target_pos, time_since_last_fixed_update_);
}

void Client::DrawImGui() noexcept {
  if (state_ == ClientState::kInGame) return;

  ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Once);

  ImGui::Begin(("Game Client " + 
      std::to_string(online_game_manager_.input_profile_id())).c_str());
  {
    switch (state_) {
      case ClientState::kConnecting:
        ImGui::TextWrapped("Connecting...");
        break;
      case ClientState::kInRoom:
        ImGui::TextWrapped("Waiting for an other player...");
        break;
      case ClientState::kInMainMenu:
        if (ImGui::Button("Join a Game", ImVec2(125, 25))) {
          network_interface_->JoinRandomOrCreateRoom();
        }
        break;
      case ClientState::kInGame:
      default:
        break;
    }
  }
  ImGui::End();
}

void Client::Deinit() noexcept {
  online_game_manager_.Deinit();
  game_renderer_.Deinit();

  //network_manager_.Disconnect();
}

void Client::OnNetworkEventReceived(const NetworkEvent& network_event) noexcept {
  online_game_manager_.PushNetworkEvent(network_event);
}

void Client::StartGame() noexcept {
  state_ = ClientState::kInGame;

  // PlayerId is in range 0-1 but ClientId is in range 1-2.
  online_game_manager_.SetPlayerId(client_id_ - 1);

  audio_manager_.PlayMusic(MusicType::kBattle);
}
