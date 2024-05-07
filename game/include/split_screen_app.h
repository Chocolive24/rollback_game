#pragma once

#include "application.h"
#include "network_manager.h"
#include "client.h"

#include <array>

class SplitScreenApp final : public Application {
public:
  void Setup() noexcept override;
  void Update() noexcept override;
  void Draw() noexcept override;
  void DrawImGui() noexcept override;
  void TearDown() noexcept override;

private:
  std::array<NetworkManager, game_constants::kMaxPlayerCount> network_managers_{
      NetworkManager{PHOTON_APP_ID, L"1.0"},
      NetworkManager{PHOTON_APP_ID, L"1.0"}
  };
  std::array<Client, game_constants::kMaxPlayerCount> clients_{};
  std::array<raylib::RenderTexture2D, game_constants::kMaxPlayerCount>
     render_targets_{};
};
