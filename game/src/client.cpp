#include "client.h"

#include <imgui.h>
#include <string>

void Client::Init(int input_profile_id) noexcept {
  network_manager_.RegisterClient(this);
  network_manager_.RegisterNetworkGameManager(&network_game_manager_);
  network_manager_.Connect();

  network_game_manager_.Init(input_profile_id);
  network_game_manager_.RegisterNetworkInterface(&network_manager_);

  game_renderer_.Init();
}

void Client::Update() noexcept {
  fixed_timer_ += raylib::GetFrameTime();
  
  network_manager_.Service();

  while (fixed_timer_ >= game_constants::kFixedDeltaTime) {
    if (state_ == ClientState::kInGame) {
      network_game_manager_.FixedUpdateCurrentFrame();
    }

    fixed_timer_ -= game_constants::kFixedDeltaTime;
  }
}

void Client::Draw(const raylib::RenderTexture2D& render_texture, 
    raylib::Vector2 render_target_pos) noexcept {
  if (state_ != ClientState::kInGame) 
      return;

  game_renderer_.Draw(render_texture, render_target_pos);
}

void Client::DrawImGui() noexcept {
  if (state_ == ClientState::kInGame) return;

  ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Once);

  ImGui::Begin(("Game Client " + 
      std::to_string(network_game_manager_.input_profile_id())).c_str());
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
          network_manager_.JoinRandomOrCreateRoom();
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
  network_game_manager_.Deinit();
  game_renderer_.Deinit();

  network_manager_.Disconnect();
}

void Client::StartGame() noexcept {
  state_ = ClientState::kInGame;

  // PlayerId is in range 0-1 but ClientId is in range 1-2.
  network_game_manager_.SetPlayerId(client_id_ - 1);
}
