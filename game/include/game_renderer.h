#pragma once

#include "game_gui.h"
#include "local_game_manager.h"
#include "raylib_wrapper.h"

/**
 * \brief GameRenderer is a class which draws the game into a given render target.
 */
class GameRenderer {
public:
  explicit GameRenderer(LocalGameManager* game_manager) noexcept;

  void Init() noexcept;
  void Draw(const raylib::RenderTexture2D& render_target,
            raylib::Vector2 render_target_pos, float time_since_last_fixed_update) noexcept;
  void Deinit() noexcept;

private:
  void UpdateCamera(const raylib::RenderTexture2D& render_target, 
	  raylib::Vector2 render_target_pos);
  void DrawWalls() const noexcept;
  void DrawProjectiles(float time_since_last_fixed_update) const noexcept;
  void DrawPlayers(float time_since_last_fixed_update) noexcept;

  LocalGameManager* game_manager_ = nullptr;
  GameGui game_gui_{};
  raylib::Camera2D camera_{};
};