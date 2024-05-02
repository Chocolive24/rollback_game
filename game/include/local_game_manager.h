#pragma once

#include "game_state.h"
#include "arena_manager.h"

/**
 * \brief LocalGameManager is a class that update the game logic.
 *
 * It is designed for local multiplayer.
 */
class LocalGameManager : public PhysicsEngine::ContactListener {
public:
  constexpr explicit LocalGameManager() noexcept = default;
  LocalGameManager(LocalGameManager&& other) noexcept = default;
  LocalGameManager& operator=(LocalGameManager&& other) noexcept = default;
  LocalGameManager(const LocalGameManager& other) noexcept = default;
  LocalGameManager& operator=(const LocalGameManager& other) noexcept = default;
  ~LocalGameManager() noexcept override = default;

  virtual void Init(int input_profile_id) noexcept;
  void FixedUpdate() noexcept;
  virtual void Deinit() noexcept;

  void SetPlayerId(const PlayerId player_id) noexcept {
    player_id_ = player_id;
  }

  void SetPlayerInput(const input::FrameInput& input, PlayerId player_id) noexcept;

  /**
   * \brief Copy is a method which copies the states of the game. It used
   * when rollback is applied to go back to a previous game state.
   * \param game_manager The game manager to be copied.
   */
  void Rollback(const LocalGameManager& game_manager) noexcept;

  [[nodiscard]] Checksum ComputeChecksum() const noexcept;

  [[nodiscard]] const PlayerManager& player_manager() const noexcept {
    return game_state_.player_manager;
  }

  [[nodiscard]] const ArenaManager& platform_manager() const noexcept {
    return arena_manager_;
  }

  [[nodiscard]] const ProjectileManager& projectile_manager() const noexcept {
    return game_state_.projectile_manager;
  }

  [[nodiscard]] PlayerId player_id() const noexcept {
    return player_id_;
  }

  [[nodiscard]] int input_profile_id() const noexcept {
    return input_profile_id_;
  }

  [[nodiscard]] bool is_finished() const noexcept { return game_state_.is_game_finished;}

protected:
  GameState game_state_{};
  ArenaManager arena_manager_{};

  int input_profile_id_ = -1;
  PlayerId player_id_ = -1;

  float fixed_frame_timer_ = game_constants::kFixedDeltaTime;

  void OnTriggerEnter(
      PhysicsEngine::ColliderRef colliderRefA,
      PhysicsEngine::ColliderRef colliderRefB) noexcept override{}
  void OnTriggerStay(PhysicsEngine::ColliderRef colliderRefA,
                     PhysicsEngine::ColliderRef colliderRefB) noexcept override{}
  void OnTriggerExit(PhysicsEngine::ColliderRef colliderRefA,
                     PhysicsEngine::ColliderRef colliderRefB) noexcept override{}
  void OnCollisionEnter(
      PhysicsEngine::ColliderRef colliderRefA,
      PhysicsEngine::ColliderRef colliderRefB) noexcept override;
  void OnCollisionExit(
      PhysicsEngine::ColliderRef colliderRefA,
      PhysicsEngine::ColliderRef colliderRefB) noexcept override {}
};