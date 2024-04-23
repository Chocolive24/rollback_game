#pragma once

#include "application.h"
#include "client_network_manager.h"
#include "game_renderer.h"
#include "rollback_manager.h"

class ClientApplication final : public Application {
public:
  void Setup() noexcept override;
  void Update() noexcept override;
  void Draw() noexcept override;
  void DrawImGui() noexcept override;
  void TearDown() noexcept override;

private:
  GameManager game_manager_{};
  GameRenderer game_renderer_{&game_manager_};
  RollbackManager rollback_manager_{};
  ClientNetworkManager network_manager_{PHOTON_APP_ID, L"1.0"};
  raylib::RenderTexture2D render_texture_{};
};
