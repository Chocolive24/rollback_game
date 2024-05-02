#pragma once

#include "World.h"
#include "player_manager.h"

/**
 * \brief GameState is a struct containing all the variables that describe
 * the state of the game.
 *
 * These variables are the one that are copied when a rollback is needed.
 */
struct GameState {
  PhysicsEngine::World world{};
  PlayerManager player_manager{};
  ProjectileManager projectile_manager{};
  bool is_game_finished = false;
};