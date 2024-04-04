#include "physics_manager.h"

void PhysicsManager::Init() noexcept { world_.Init(); }

void PhysicsManager::FixedUpdate() noexcept {
  world_.Update(1/50.f);
}
