#pragma once

#include "application.h"
#include "client_network_manager.h"
#include "game_renderer.h"
#include "network_game_manager.h"
#include "rollback_manager.h"

enum class AppState {
  kConnecting,
  kInMainMenu,
  kInRoom,
  kInGame
};

class ClientApplication final : public Application {
public:
  void Setup() noexcept override;
  void Update() noexcept override;
  void Draw() noexcept override;
  void DrawImGui() noexcept override;
  void TearDown() noexcept override;

  void StartGame() noexcept;

  void SetState(const AppState state) noexcept { state_ = state; }

  [[nodiscard]] ClientId client_id() const noexcept { return client_id_; }
  void SetClientId(const ClientId client_id) noexcept {
    client_id_ = client_id;
  }


private:
  NetworkGameManager network_game_manager_{};
  RollbackManager rollback_manager_{};
  ClientNetworkManager network_manager_{PHOTON_APP_ID, L"1.0"};

  GameRenderer game_renderer_{&network_game_manager_};
  raylib::RenderTexture2D render_texture_{};

  float fixed_timer_ = game_constants::kFixedDeltaTime;
  AppState state_ = AppState::kConnecting;

  ClientId client_id_ = game_constants::kInvalidClientId;
};
