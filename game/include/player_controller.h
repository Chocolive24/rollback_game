#pragma once

#include "World.h"

/**
 * \brief PlayerController is a class which update the state of a player.
 */
class PlayerController {
public:
  void RegisterWorld(PhysicsEngine::World* world) noexcept { world_ = world; }

  void Init() noexcept;
  void Update() const noexcept;
  void PollInputs() noexcept;

  PhysicsEngine::ColliderRef col_ref_{};
  PhysicsEngine::ColliderRef can_jump_col_ref_{};
  bool can_jump_ = true;

 private:
 PhysicsEngine::World* world_ = nullptr;

 Math::Vec2F move_direction_{};
 bool is_jumping_ = false;

};