#pragma once

#include "Vec2.h"

/**
 * \brief game_constants is a namespace that stores all the constants variables
 * of the game.
 */
namespace game_constants {
  constexpr float kFixedDeltaTime = 1 / 50.f;

  constexpr std::uint8_t kMaxPlayerCount = 2;

  constexpr float kPlayerColRadius = 0.5f;
  constexpr float kPlayerSpeedMoveFactor = 5.f;
  constexpr Math::Vec2F kPlayerStartPos = Math::Vec2F(1, 1);
}
