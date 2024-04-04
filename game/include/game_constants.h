#pragma once

#include "Vec2.h"

/**
 * \brief game_constants is a namespace that stores all the constants variables
 * of the game.
 */
namespace game_constants {
   constexpr float kFixedDeltaTime = 1 / 50.f;

   static constexpr float kPlayerColRadius = 0.5f;
   static constexpr float kPlayerSpeedMoveFactor = 5.f;
   static constexpr Math::Vec2F kPlayerStartPos = Math::Vec2F(1, 1);
}
