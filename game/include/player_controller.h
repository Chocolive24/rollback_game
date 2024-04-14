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

  void Shoot() noexcept;

  void OnTriggerEnter(PhysicsEngine::ColliderRef colliderRefA,
                      PhysicsEngine::ColliderRef colliderRefB) noexcept;

  [[nodiscard]] Math::Vec2F GetPlayerPosition() const noexcept;
  [[nodiscard]] Math::Vec2F GetJumpColliderPosition() const noexcept;
  [[nodiscard]] Math::RectangleF GetMainColliderShape() const noexcept;
  [[nodiscard]] Math::CircleF GetJumpColliderShape() const noexcept;

  bool can_jump_ = true;
  bool is_jumping_ = false;

private:
 PhysicsEngine::World* world_ = nullptr;
 PhysicsEngine::ColliderRef main_col_ref_{};
 PhysicsEngine::ColliderRef jump_col_ref_{};
 Math::Vec2F move_direction_{};
};