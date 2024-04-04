#pragma once
#include "Collider.h"
#include "World.h"

/**
 * \brief PhysicsManager is the class which updates the game physics.
 */
class PhysicsManager {
public:
  void Init() noexcept;
  void FixedUpdate() noexcept;
  void Deinit() noexcept;

  
private:
 PhysicsEngine::World world_{};
};