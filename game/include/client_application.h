#pragma once

#include "application.h"
#include "game_renderer.h"

class ClientApplication final : public Application {
public:
  void Setup() noexcept override;
  void Update() noexcept override;
  void Draw() noexcept override;
  void DrawImGui() noexcept override;
  void TearDown() noexcept override;

private:
  GameRenderer game_renderer_{};
};
