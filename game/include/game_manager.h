#pragma once

#include "platforms_manager.h"
#include "player_manager.h"
#include "World.h"

class RollbackManager;

/**
 * \brief Game is a struct containing all the variables that describe
 * the state of the game. These variables are the one that are copied
 * when a rollback is needed.
 */
struct Game {
  PhysicsEngine::World world;
  std::array<inputs::PlayerInput, game_constants::kMaxPlayerCount> player_inputs{};
};

/**
 * \brief GameManager is a class that update the game logic.
 */
class GameManager : public PhysicsEngine::ContactListener {
public:
  constexpr explicit GameManager() noexcept = default;
  GameManager(GameManager&& other) noexcept = default;
  GameManager& operator=(GameManager&& other) noexcept = default;
  GameManager(const GameManager& other) noexcept = default;
  GameManager& operator=(const GameManager& other) noexcept = default;
  ~GameManager() noexcept override = default;

  void Init(PlayerId player_id, int input_profile_id) noexcept;
  void Update() noexcept;
  void FixedUpdate(FrameNbr frame_nbr) noexcept;
  void Deinit() noexcept;

  void RegisterRollbackManager(RollbackManager* rollback_manager) noexcept {
    rollback_manager_ = rollback_manager;
  }

  /**
   * \brief Copy is a method which copies the states of the game. It used
   * when rollback is applied to go back to a previous game state.
   * \param game_manager The game manager to be copied.
   */
  void Copy(const GameManager& game_manager) noexcept;

  /**
   * \brief Copy is a method which copies the states of the game. It used
   * when rollback is applied to go back to a previous game state.
   * \param game The game to be copied.
   */
  //void Copy(const Game& game) noexcept { game_ = game; }

  [[nodiscard]] int ComputeChecksum() const noexcept;

  [[nodiscard]] const PlayerManager& player_manager() const noexcept {
    return player_manager_;
  }

  [[nodiscard]] const PlatformManager& platform_manager() const noexcept {
    return platform_manager_;
  }

  [[nodiscard]] const ProjectileManager& projectile_manager() const noexcept {
    return projectile_manager_;
  }

  [[nodiscard]] PlayerId player_id() const noexcept {
    return player_id_;
  }

  [[nodiscard]] int input_profile_id() const noexcept {
    return input_profile_id_;
  }

protected:
  //Game game_{};
  PhysicsEngine::World world_{};

  RollbackManager* rollback_manager_ = nullptr;

  PlayerManager player_manager_{};
  ProjectileManager projectile_manager_{};
  PlatformManager platform_manager_{};

  int input_profile_id_ = -1;
  PlayerId player_id_ = -1;

  float fixed_timer_ = game_constants::kFixedDeltaTime;

  void OnTriggerEnter(
      PhysicsEngine::ColliderRef colliderRefA,
      PhysicsEngine::ColliderRef colliderRefB) noexcept override;
  void OnTriggerStay(PhysicsEngine::ColliderRef colliderRefA,
                     PhysicsEngine::ColliderRef colliderRefB) noexcept override;
  void OnTriggerExit(PhysicsEngine::ColliderRef colliderRefA,
                     PhysicsEngine::ColliderRef colliderRefB) noexcept override;
  void OnCollisionEnter(
      PhysicsEngine::ColliderRef colliderRefA,
      PhysicsEngine::ColliderRef colliderRefB) noexcept override {}
  void OnCollisionExit(
      PhysicsEngine::ColliderRef colliderRefA,
      PhysicsEngine::ColliderRef colliderRefB) noexcept override {}
};