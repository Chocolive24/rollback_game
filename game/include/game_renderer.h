#pragma once

#include "game_manager.h"

/**
 * \brief GameRenderer is a class that inherits from GameManager to update and render
 * the state of the game.
 */
class GameRenderer final : public GameManager {
public:
  void Init() noexcept override;
  void Draw() noexcept;
  void Deinit() noexcept override;
};