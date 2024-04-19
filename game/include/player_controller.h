#pragma once

#include "game_constants.h"
#include "inputs.h"
#include "World.h"

/**
 * \brief Player is a struct containing all the variables that describe
 * the state of the player. These variables are the one that are copied
 * when a rollback is needed.
 */
struct Player {
  Math::Vec2F position{0.f, 0.f};
  inputs::PlayerInput input = 0;
  PlayerId id = -1;
};

/**
 * \brief PlayerManager is a class which update the state of the players.
 */
class PlayerManager {
public:
  void RegisterWorld(PhysicsEngine::World* world) noexcept { world_ = world; }

  void Init() noexcept;
  void FixedUpdate() noexcept;
  void PollInputs() noexcept;

  void Shoot() noexcept;

  /**
   * \brief Copy is a method which copies the states of the players. It used when
   * rollback is applied to go back to the previous players state.
   * \param player_manager The player manager to be copied.
   */
  void Copy(const PlayerManager& player_manager) noexcept;

  [[nodiscard]] int ComputeChecksum() const noexcept;

  void OnTriggerEnter(PhysicsEngine::ColliderRef colliderRefA,
                      PhysicsEngine::ColliderRef colliderRefB) noexcept;

  void SetPlayerInput(inputs::PlayerInput input, PlayerId player_id);

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