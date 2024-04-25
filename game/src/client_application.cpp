#include "client_application.h"
#include "engine.h"

#include <imgui.h>

void ClientApplication::Setup() noexcept {
  network_manager_.RegisterClientApplication(this);
  network_manager_.RegisterNetworkGameManager(&network_game_manager_);
  network_manager_.Connect();
}

void ClientApplication::Update() noexcept {
  fixed_timer_ += raylib::GetFrameTime();

  network_manager_.Service();

  while (fixed_timer_ >= game_constants::kFixedDeltaTime) {
    if (state_ == AppState::kInGame) {
      rollback_manager_.IncreaseCurrentFrame();

      network_game_manager_.SendInputEvent();
      network_game_manager_.FixedUpdate(rollback_manager_.current_frame());
    }

    fixed_timer_ -= game_constants::kFixedDeltaTime;
  }
}

void ClientApplication::Draw() noexcept {
  if (state_ != AppState::kInGame) return;

  if (raylib::IsWindowResized()) {
    const auto new_tex_size = Engine::window_size();
    UnloadRenderTexture(render_texture_);
    render_texture_ = raylib::LoadRenderTexture(new_tex_size.X, new_tex_size.Y);
  }

  game_renderer_.Draw(render_texture_);

  raylib::DrawTextureRec(render_texture_.texture,
      raylib::Rectangle{0, 0, static_cast<float>(render_texture_.texture.width),
                        static_cast<float>(-render_texture_.texture.height)},
      raylib::Vector2{0.f, 0.f}, raylib::kWhite);
}

void ClientApplication::DrawImGui() noexcept {
  if (state_ == AppState::kInGame) return;

  ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Once);

  ImGui::Begin("Game Client");
  {
    switch (state_)
    {
    case AppState::kConnecting:
      ImGui::TextWrapped("Connecting...");
      break;
    case AppState::kInRoom:
      ImGui::TextWrapped("Waiting for an other player...");
      break;
    case AppState::kInMainMenu:
      if (ImGui::Button("Join a Game", ImVec2(125, 25))) {
        network_manager_.JoinRandomOrCreateRoom();
      }
      break;
    case AppState::kInGame:
    default:
      break;
    }
  }
  ImGui::End();
}

void ClientApplication::TearDown() noexcept {
  network_game_manager_.Deinit();
  game_renderer_.Deinit();

  network_manager_.Disconnect();
}

void ClientApplication::StartGame() noexcept {
  state_ = AppState::kInGame;

  // PlayerId is in range 0-1 but ClientId is in range 1-2.
  network_game_manager_.Init(client_id_ - 1, game_constants::kLocalPlayer1InputId);
  network_game_manager_.RegisterRollbackManager(&rollback_manager_);
  network_game_manager_.RegisterNetworkInterface(&network_manager_);
  rollback_manager_.RegisterGameManager(&network_game_manager_);

  game_renderer_.Init();
  render_texture_ = raylib::LoadRenderTexture(raylib::GetScreenWidth(),
                                              raylib::GetScreenHeight());
}
