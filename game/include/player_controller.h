#pragma once

#include "World.h"

/**
 * \brief PlayerController is a class which update the state of a player.
 */
class PlayerController final : public PhysicsEngine::ContactListener {
public:
  constexpr explicit PlayerController(PhysicsEngine::World* world) noexcept;

  void Init() noexcept;
  void Update() const noexcept;
  void PollInputs() noexcept;

  PhysicsEngine::ColliderRef col_ref_{};

private:
 PhysicsEngine::World* world_ = nullptr;
 Math::Vec2F move_direction_{};

  void OnTriggerEnter(PhysicsEngine::ColliderRef colliderRefA,
                      PhysicsEngine::ColliderRef colliderRefB) noexcept override {}
 void OnTriggerStay(PhysicsEngine::ColliderRef colliderRefA,
                    PhysicsEngine::ColliderRef colliderRefB) noexcept override {}
 void OnTriggerExit(PhysicsEngine::ColliderRef colliderRefA,
                    PhysicsEngine::ColliderRef colliderRefB) noexcept override {}
 void OnCollisionEnter(PhysicsEngine::ColliderRef colliderRefA,
                       PhysicsEngine::ColliderRef colliderRefB) noexcept override {}
 void OnCollisionExit(PhysicsEngine::ColliderRef colliderRefA,
                      PhysicsEngine::ColliderRef colliderRefB) noexcept override {}
};

constexpr PlayerController::PlayerController(PhysicsEngine::World* world) noexcept {
  world_ = world;
}