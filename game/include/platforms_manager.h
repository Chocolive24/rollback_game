#pragma once

#include "game_constants.h"
#include "World.h"

#include <array>

class PlatformManager {
public:
  void Init(PhysicsEngine::World* world) noexcept;

  [[nodiscard]] Math::Vec2F GetPlatformPosition(
      const std::size_t& idx)const noexcept;

  [[nodiscard]] Math::RectangleF GetPlatformShape(
      const std::size_t& idx) const noexcept;

private:
  std::array<PhysicsEngine::ColliderRef,
    game_constants::kPlatformCount>platform_col_refs{};

  PhysicsEngine::World* world_ = nullptr;
};