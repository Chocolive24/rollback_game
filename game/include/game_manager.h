#pragma once

#include "game_constants.h"
#include "player_controller.h"
#include "World.h"

/**
 * \brief GameManager is a class that update the game logic.
 * A Renderer can inherit from it to be able to draw the game state on screen.
 */
class GameManager {
public:
  constexpr explicit GameManager() noexcept = default;
  GameManager(GameManager&& other) noexcept = default;
  GameManager& operator=(GameManager&& other) noexcept = default;
  GameManager(const GameManager& other) noexcept = default;
  GameManager& operator=(const GameManager& other) noexcept = default;
  virtual ~GameManager() noexcept = default;

  virtual void Init() noexcept;
  void Update() noexcept;
  virtual void Deinit() noexcept;

protected:
  PhysicsEngine::World world_{};
  PlayerController player_controller_{&world_};

private:
  void PollInputs() noexcept;
};