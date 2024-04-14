#pragma once

#include "game_constants.h"
#include "platforms_manager.h"
#include "player_controller.h"
#include "World.h"

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
  void Update() noexcept;
  virtual void Deinit() noexcept;

  void SetPlayerInput(inputs::PlayerInput input, std::size_t idx);

  [[nodiscard]] const PlayerManager& player_controller() const noexcept {
    return player_manager_;
  }

  [[nodiscard]] const PlatformManager& platform_manager() const noexcept {
    return platform_manager_;
  }

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
  void PollInputs() noexcept;

  int local_player_id_ = -1;
};