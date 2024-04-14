#pragma once

#include "application.h"
#include "game_constants.h"
#include "simulation_client.h"

/**
 * \brief DebugApp is a class which simulates the network communications with
 * mock clients and server. It enables to debug easily the game without needing
 * network connection.
 */
class SimulationApp final : public Application {
 public:
  void Setup() noexcept override;
  void Update() noexcept override;
  void Draw() noexcept override;
  void DrawImGui() noexcept override;
  void TearDown() noexcept override;

 private:
  std::array<SimulationClient, game_constants::kMaxPlayerCount> clients_{};
  std::array<raylib::RenderTexture2D, game_constants::kMaxPlayerCount>
      render_targets_{};
};
