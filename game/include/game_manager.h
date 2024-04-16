#pragma once

#include "rollback_manager.h"
#include "platforms_manager.h"
#include "player_controller.h"
#include "World.h"

/**
 * \brief Game is a struct containing all the variables that describe
 * the state of the game. These variables are the one that are copied
 * when a rollback is needed.
 */
struct Game {
  //TODO: add varibales mdr.
};

/**
 * \brief GameManager is a class that update the game logic.
 * A Renderer can inherit from it to be able to draw the game state on screen.
 */
class GameManager : public PhysicsEngine::ContactListener {
public:
  constexpr explicit GameManager() noexcept = default;
  GameManager(GameManager&& other) noexcept = default;
  GameManager& operator=(GameManager&& other) noexcept = default;
  GameManager(const GameManager& other) noexcept = default;
  GameManager& operator=(const GameManager& other) noexcept = default;
  ~GameManager() noexcept override = default;

  virtual void Init(int local_player_id) noexcept;
  void FixedUpdate() noexcept;
  virtual void Deinit() noexcept;

  /**
   * \brief Copy is a method which copies the states of the game. It used
   * when rollback is applied to go back to a previous game state. \param
   * game_manager The game manager to be copied.
   */
  void Copy(const GameManager& game_manager) noexcept;

  void SetPlayerInput(inputs::FrameInput input, PlayerId player_id);
  void SetRemotePlayerInput(inputs::FrameInput input, PlayerId player_id);

  [[nodiscard]] const PlayerManager& player_manager() const noexcept {
    return player_manager_;
  }

  [[nodiscard]] const PlatformManager& platform_manager() const noexcept {
    return platform_manager_;
  }

  RollbackManager rollback_manager_{};

protected:
  PhysicsEngine::World world_{};
  
  PlayerManager player_manager_;
  PlatformManager platform_manager_{};

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

private:
  int local_player_id_ = -1;
};