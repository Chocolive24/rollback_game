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
  GameManager game_manager_{};
  GameRenderer game_renderer_{&game_manager_};
  raylib::RenderTexture2D render_texture_{};
};
