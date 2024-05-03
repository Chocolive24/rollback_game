#pragma once

#include "game_constants.h"
#include "input.h"
#include "projectile_manager.h"
#include "World.h"

/**
 * \brief Player is a struct containing all the variables that describe
 * the state of the player. These variables are the one that are copied
 * when a rollback is needed.
 */
struct Player {
  PhysicsEngine::ColliderRef main_col_ref{};
  PhysicsEngine::ColliderRef jump_col_ref{};
  Math::Vec2F dir_to_mouse{};
  float shoot_timer = 0.f;
  float damage_timer = 0.f;
  float spin_timer = 0.f;
  std::int8_t hp = 5;
  input::PlayerInput input = 0;
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
  void Rollback(const PlayerManager& player_manager) noexcept;

  [[nodiscard]] Checksum ComputeChecksum() const noexcept;

  void OnCollisionEnter(PhysicsEngine::ColliderRef colliderRefA,
                      PhysicsEngine::ColliderRef colliderRefB) noexcept;

  void SetPlayerInput(const input::FrameInput& input, PlayerId player_id);

  void ApplyOneDamageToPlayer(std::size_t player_idx) noexcept;

  [[nodiscard]] std::int8_t GetPlayerHp(std::size_t idx) const noexcept;
  [[nodiscard]] Math::Vec2F GetPlayerPosition(std::size_t idx) const noexcept;
  [[nodiscard]] Math::Vec2F GetPlayerVelocity(std::size_t idx) const noexcept;
  [[nodiscard]] PhysicsEngine::ColliderRef GetPlayerColRef(
      std::size_t idx) const noexcept;
  [[nodiscard]] Math::Vec2F GetJumpColliderPosition(std::size_t idx) const noexcept;
  [[nodiscard]] Math::CircleF GetJumpColliderShape(std::size_t idx) const noexcept;

  [[nodiscard]] bool IsPlayerWalking(std::size_t idx) const noexcept;
  [[nodiscard]] bool IsPlayerSpinning(std::size_t idx) const noexcept;
  [[nodiscard]] bool IsPlayerHurt(std::size_t idx) const noexcept;
  [[nodiscard]] bool IsPlayerFacingRight(std::size_t idx) const noexcept;

  void LaunchSpinTimer(std::size_t idx) noexcept;

  static constexpr float kPlayerDamping = 0.75f;
  static constexpr float kPlayerMass = 1.f;

private:
 void Move(const Player& player) const noexcept;
 void Shoot(const Player& player) const noexcept;
 std::array<Player, game_constants::kMaxPlayerCount> players_{};
 PhysicsEngine::World* world_ = nullptr;
 ProjectileManager* projectile_manager_ = nullptr;

  static constexpr float kShootCooldown = 0.5f;
  static constexpr float kDamageCooldown = 2.f;
  static constexpr float kSpinCooldown = 1.f;
};