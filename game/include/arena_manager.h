#pragma once

#include "game_constants.h"
#include "World.h"

#include <array>

class ArenaManager {
public:
  void Init(PhysicsEngine::World* world) noexcept;

  [[nodiscard]] PhysicsEngine::ColliderRef GetWallColRef(std::size_t idx) const noexcept {
    return wall_col_refs[idx];
  }

  static constexpr std::array<Math::Vec2F, game_constants::kArenaBorderWallCount>
      border_wall_positions {

        Math::Vec2F(0.f, 3.50f), // left
        Math::Vec2F(6.40f, 0.f), // up
        Math::Vec2F(12.80f, 3.50f), // right
        Math::Vec2F(6.40f, 7.20f), // down
      };

  static constexpr std::array<Math::Vec2F, game_constants::kArenaSquareWallCount>
      square_wall_positions{
        Math::Vec2F(12.80f * 0.275f, 7.20f * 0.275f),  // up-left
          Math::Vec2F(12.80f * 0.725f, 7.20f * 0.275f),  // up-right
          Math::Vec2F(12.80f * 0.275f, 7.20f * 0.725f),  // down-left
          Math::Vec2F(12.80f * 0.725f, 7.20f * 0.725f)   // down-right
      };

  static constexpr std::array<Math::RectangleF, game_constants::kArenaBorderWallCount>
      wall_shapes {
          // Borders.
          Math::RectangleF(-game_constants::kVerticalWallSize * 0.5f, 
          game_constants::kVerticalWallSize * 0.5f),  // left

          Math::RectangleF(-game_constants::kHorizontalWallSize * 0.5f,
                           game_constants::kHorizontalWallSize * 0.5f),  // up

          Math::RectangleF(-game_constants::kVerticalWallSize * 0.5f,
                           game_constants::kVerticalWallSize * 0.5f),  // right

          Math::RectangleF(-game_constants::kHorizontalWallSize * 0.5f,
                           game_constants::kHorizontalWallSize * 0.5f),  // down
      };

private:
  static constexpr auto kTotalWallCount = game_constants::kArenaBorderWallCount +
                                   game_constants::kArenaSquareWallCount;

  std::array<PhysicsEngine::ColliderRef, kTotalWallCount> wall_col_refs{};

  PhysicsEngine::World* world_ = nullptr;
};