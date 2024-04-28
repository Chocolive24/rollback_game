#pragma once

#include "types.h"
#include "Vec2.h"

/**
 * \brief game_constants is a namespace that stores all the constants variables
 * of the game.
 */
namespace game_constants {
  constexpr float kFixedDeltaTime = 1 / 50.f;

  constexpr int kLocalPlayer1InputId = 0;
  constexpr int kLocalPlayer2InputId = 0;

  constexpr ClientId kInvalidClientId = -1;

  constexpr Math::Vec2I kGameScreenSize(1280, 720);

  constexpr std::uint8_t kMaxPlayerCount = 2;

  /**
   * \brief kGameBodyCount is the total number of bodies needed by the game.
   *
   * It represents the 100 projectiles pool, the 2 players and the 4 arena walls.
   */
  constexpr std::int8_t kGameBodyCount = 106;

  constexpr float kPlayerMainColLength = 0.65f;
  constexpr float kPlayerJumpColRadius = 0.1f;
  constexpr Math::Vec2F kPlayerJumpColOffset(0.f, kPlayerMainColLength * 0.5f);
  constexpr float kPlayerSpeedMoveFactor = 5.f;
  constexpr float kPlayerJumpMagnitude = 200.f;
  constexpr Math::Vec2F kPlayer1StartPos = Math::Vec2F(5.40f, 1.60f);
  constexpr Math::Vec2F kPlayer2StartPos = Math::Vec2F(7.40f, 1.60f);

  constexpr std::uint8_t kArenaWallCount = 4;
  constexpr Math::Vec2I kArenaSize = Math::Vec2I(7, 7);
  constexpr Math::Vec2F kHorizontalWallSize = Math::Vec2F(kArenaSize.X, 0.25f);
  constexpr Math::Vec2F kVerticalWallSize = Math::Vec2F(0.25f, kArenaSize.Y);
  constexpr Math::Vec2F kMainPlatformPos = Math::Vec2F(6.40f, 5.f);

}
