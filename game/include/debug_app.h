#pragma once

#include "app_interface.h"
#include "debug_client.h"

/**
 * \brief DebugApp is a class which simulates the network communications with
 * mock clients and server. It enables to debug easily the game without needing
 * network connection.
 */
class DebugApp final : public Application {
public:
  void Setup() noexcept override;
  void Update() noexcept override;
  void Draw() noexcept override;
  void DrawImGui() noexcept override;
  void TearDown() noexcept override;

private:
  DebugClient client_1_{};
  DebugClient client_2_{};
};
