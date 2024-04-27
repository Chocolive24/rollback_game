#pragma once

#include "application.h"
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
  std::array<Client, game_constants::kMaxPlayerCount> clients_{};
  std::array<raylib::RenderTexture2D, game_constants::kMaxPlayerCount>
     render_targets_{};
};
