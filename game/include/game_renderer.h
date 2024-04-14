#pragma once

#include "game_manager.h"

/**
 * \brief GameRenderer is a class which draws the game into the last render target
 * activated.
 */
class GameRenderer {
public:
  explicit GameRenderer(GameManager* game_manager) noexcept;
  void Init() const noexcept;
  void Draw() const noexcept;
  void Deinit() const noexcept;

private:
  void DrawPlatforms() const noexcept;
  void DrawPlayer() const noexcept;

  GameManager* game_manager_ = nullptr;
};