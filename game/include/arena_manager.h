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

  static constexpr std::array<Math::Vec2F, game_constants::kArenaWallCount>
      wall_positions {
        Math::Vec2F(0.f, 3.50f), // left
        Math::Vec2F(6.40f, 0.f), // up
        Math::Vec2F(12.80f, 3.50f), // right
        Math::Vec2F(6.40f, 7.0f), // down
      };

  static constexpr std::array<Math::RectangleF, game_constants::kArenaWallCount>
      wall_shapes {
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
  std::array<PhysicsEngine::ColliderRef,
    game_constants::kArenaWallCount> wall_col_refs{};

  PhysicsEngine::World* world_ = nullptr;
};