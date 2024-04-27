#pragma once

#include "local_game_manager.h"
#include "raylib_wrapper.h"

/**
 * \brief GameRenderer is a class which draws the game into a given render target.
 */
class GameRenderer {
public:
  explicit GameRenderer(LocalGameManager* game_manager) noexcept;

  void Init() noexcept;
  void Draw(const raylib::RenderTexture2D& render_target) noexcept;
  void Deinit() noexcept;

private:
  void UpdateCamera(const raylib::RenderTexture2D& render_target);
  void DrawPlatforms() const noexcept;
  void DrawProjectiles() const noexcept;
  void DrawPlayer() const noexcept;

  LocalGameManager* game_manager_ = nullptr;
  raylib::Camera2D camera_{};
};