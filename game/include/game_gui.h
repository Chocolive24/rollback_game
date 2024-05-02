#pragma once

#include "local_game_manager.h"
#include "raylib_wrapper.h"

class GameGui {
public:
  void Init(LocalGameManager* local_game_manager) noexcept {
   local_game_manager_ = local_game_manager;
  }

  void Draw(const raylib::RenderTexture2D& render_target) const noexcept;

 private:
  LocalGameManager* local_game_manager_ = nullptr;

  static constexpr float kPlayer1LifeUiPercentageX = 0.10f;
  static constexpr float kPlayer2LifeUiPercentageX = 1 - kPlayer1LifeUiPercentageX;
  static constexpr float kPlayersLivesUiPercentageY = 0.075f;
};