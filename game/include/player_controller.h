#pragma once

#include "game_constants.h"
#include "inputs.h"
#include "World.h"

/**
 * \brief Player is a struct containing all the variables that describe
 * the state of the player. These variables are the on that are copied
 * when a rollback is needed.
 */
struct Player {
  Math::Vec2F position{0.f, 0.f};
  Math::Vec2F velocity{0.f, 0.f};
  inputs::PlayerInput input = 0;
  // bool can_jump_ = true;
  // bool is_jumping_ = false;
};

/**
 * \brief PlayerManager is a class which update the state of the players.
 */
class PlayerManager {
public:
  void RegisterWorld(PhysicsEngine::World* world) noexcept { world_ = world; }

  void Init() noexcept;
  void Update() noexcept;
  void PollInputs() noexcept;

  void Shoot() noexcept;

  void OnTriggerEnter(PhysicsEngine::ColliderRef colliderRefA,
                      PhysicsEngine::ColliderRef colliderRefB) noexcept;

  void SetPlayerInput(inputs::PlayerInput input, std::size_t idx);

  [[nodiscard]] Math::Vec2F GetPlayerPosition(std::size_t idx) const noexcept;
  [[nodiscard]] Math::Vec2F GetJumpColliderPosition(
      std::size_t idx) const noexcept;
  [[nodiscard]] Math::CircleF GetJumpColliderShape(
      std::size_t idx) const noexcept;

  //bool can_jump_ = true;
  //bool is_jumping_ = false;

private:
 std::array<Player, game_constants::kMaxPlayerCount> players_{};
 PhysicsEngine::World* world_ = nullptr;
 PhysicsEngine::ColliderRef main_col_ref_{};
 PhysicsEngine::ColliderRef jump_col_ref_{};
 Math::Vec2F move_direction_{};
};