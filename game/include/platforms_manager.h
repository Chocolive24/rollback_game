#pragma once

#include "game_constants.h"
#include "World.h"

#include <array>

class PlatformManager {
public:
  void Init(PhysicsEngine::World* world) noexcept;

  //[[nodiscard]] Math::Vec2F GetPlatformPosition(
  //    const std::size_t& idx)const noexcept;

  //[[nodiscard]] Math::RectangleF GetPlatformShape(
  //    const std::size_t& idx) const noexcept;

  static constexpr std::array<Math::Vec2F, game_constants::kArenaWallCount>
      wall_positions{
        Math::Vec2F(3.50, 3.50f), // left
        Math::Vec2F(7.00, 0.f), // up
        Math::Vec2F(10.50f, 3.50f), // right
        Math::Vec2F(7.0, 7.0f), // down
      };

  static constexpr std::array<Math::RectangleF, game_constants::kArenaWallCount>
      wall_shapes{
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