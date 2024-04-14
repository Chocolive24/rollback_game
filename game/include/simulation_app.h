#pragma once

#include "application.h"
#include "game_constants.h"
#include "game_renderer.h"
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
  GameManager game_manager_{};
  GameRenderer game_renderer_{&game_manager_};

  std::array<SimulationClient, game_constants::kMaxPlayerCount> clients_{};
};
