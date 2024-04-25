#pragma once

#include "game_constants.h"
#include "inputs.h"
#include "projectile_manager.h"
#include "World.h"

/**
 * \brief Player is a struct containing all the variables that describe
 * the state of the player. These variables are the one that are copied
 * when a rollback is needed.
 */
struct Player {
  inputs::PlayerInput input = 0;
  float shoot_timer_ = 0.f;
  PhysicsEngine::ColliderRef main_col_ref{};
  PhysicsEngine::ColliderRef jump_col_ref{};
};

/**
 * \brief PlayerManager is a class which update the state of the players.
 */
class PlayerManager {
public:
  void RegisterWorld(PhysicsEngine::World* world) noexcept { world_ = world; }
  void RegisterProjectileManager(ProjectileManager* proj_manager) noexcept {
    projectile_manager_ = proj_manager;
  }

  void Init() noexcept;
  void FixedUpdate() noexcept;

  /**
   * \brief Copy is a method which copies the states of the players. It used when
   * rollback is applied to go back to the previous players state.
   * \param player_manager The player manager to be copied.
   */
  void Copy(const PlayerManager& player_manager) noexcept;

  [[nodiscard]] Checksum ComputeChecksum() const noexcept;

  void OnTriggerEnter(PhysicsEngine::ColliderRef colliderRefA,
                      PhysicsEngine::ColliderRef colliderRefB) noexcept;

  void SetPlayerInput(inputs::PlayerInput input, PlayerId player_id);

  [[nodiscard]] Math::Vec2F GetPlayerPosition(std::size_t idx) const noexcept;
  [[nodiscard]] Math::Vec2F GetJumpColliderPosition(
      std::size_t idx) const noexcept;
  [[nodiscard]] Math::CircleF GetJumpColliderShape(
      std::size_t idx) const noexcept;

private:
 void Move(const Player& player) const noexcept;
 void Shoot(const Player& player) const noexcept;
 std::array<Player, game_constants::kMaxPlayerCount> players_{};
 PhysicsEngine::World* world_ = nullptr;
 ProjectileManager* projectile_manager_ = nullptr;

  static constexpr float kShootCooldown = 0.5f;
};