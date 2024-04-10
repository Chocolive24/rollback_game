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

  virtual void Init() noexcept;
  void Update() noexcept;
  virtual void Deinit() noexcept;

protected:
  PhysicsEngine::World world_{};
  PlayerController player_controller_;
  //std::array<PlayerController, game_constants::kMaxPlayerCount> player_controllers_;
  PlatformsManager platforms_manager_{};

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
};