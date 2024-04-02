#pragma once

#include "ContactListener.h"
#include "Timer.h"
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
  ~GameManager() override = default;

  virtual void Init() noexcept;
  void PollInputs() noexcept;
  void Update() noexcept;
  virtual void Deinit() noexcept;

protected:
  PhysicsEngine::World world_{};
  PhysicsEngine::ColliderRef ball_col_ref_{};
  static constexpr float kFixedDeltaTime = 1 / 50.f;
  Timer timer_{};

  Math::Vec2F move_direction_{};
  static constexpr float kMaxSpeedAmplitude = 0.5f;

  static constexpr Math::Vec2F kStartBallPos = Math::Vec2F(1, 1);
  static constexpr float kBallRadius = 0.5f;

  void OnTriggerEnter(PhysicsEngine::ColliderRef colliderRefA,
                      PhysicsEngine::ColliderRef colliderRefB) noexcept override{}
  void OnTriggerStay(PhysicsEngine::ColliderRef colliderRefA,
                     PhysicsEngine::ColliderRef colliderRefB) noexcept override{}
  void OnTriggerExit(PhysicsEngine::ColliderRef colliderRefA,
                     PhysicsEngine::ColliderRef colliderRefB) noexcept override{}
  void OnCollisionEnter(PhysicsEngine::ColliderRef colliderRefA,
                        PhysicsEngine::ColliderRef colliderRefB) noexcept override{}
  void OnCollisionExit(PhysicsEngine::ColliderRef colliderRefA,
                       PhysicsEngine::ColliderRef colliderRefB) noexcept override{}

};
