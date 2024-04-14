#pragma once

#include "game_manager.h"
#include "raylib_wrapper.h"

/**
 * \brief GameRenderer is a class which draws the game into the last render target
 * activated.
 */
class GameRenderer {
public:
  explicit GameRenderer(GameManager* game_manager) noexcept;
  void Init() noexcept;
  void Draw(const raylib::RenderTexture2D& render_target) noexcept;
  void Deinit() noexcept;

private:
  void UpdateCamera(const raylib::RenderTexture2D& render_target);
  void DrawPlatforms() const noexcept;
  void DrawPlayer() const noexcept;

  GameManager* game_manager_ = nullptr;
  raylib::Camera2D camera_;
};