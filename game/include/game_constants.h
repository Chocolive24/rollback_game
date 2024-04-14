#pragma once

#include "Vec2.h"

/**
 * \brief game_constants is a namespace that stores all the constants variables
 * of the game.
 */
namespace game_constants {
  constexpr float kFixedDeltaTime = 1 / 50.f;

  constexpr Math::Vec2I kGameScreenSize(1280, 720);

  constexpr std::uint8_t kMaxPlayerCount = 2;

  constexpr float kPlayerMainColLength = 0.65f;
  constexpr float kPlayerJumpColRadius = 0.1f;
  constexpr Math::Vec2F kPlayerJumpColOffset(0.f, kPlayerMainColLength * 0.5f);
  constexpr float kPlayerSpeedMoveFactor = 5.f;
  constexpr float kPlayerJumpMagnitude = 200.f;
  constexpr Math::Vec2F kPlayerStartPos = Math::Vec2F(6.40f, 1.60f);

  constexpr std::uint8_t kPlatformCount = 1;
  constexpr Math::Vec2F kMainPlatformColSize = Math::Vec2F(6.5f, 0.75f);
  constexpr Math::Vec2F kMainPlatformPos = Math::Vec2F(6.40f, 5.f);

}
