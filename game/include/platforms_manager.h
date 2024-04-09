#pragma once

#include "game_constants.h"
#include "World.h"

#include <array>

class PlatformsManager {
public:
  void Init(PhysicsEngine::World* world) noexcept;
  std::array<PhysicsEngine::ColliderRef, game_constants::kPlatformCount>
      platform_col_refs{};

private:
  PhysicsEngine::World* world_ = nullptr;
};