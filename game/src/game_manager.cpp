#include "game_manager.h"

#include <iostream>
#include <raylib.h>

void GameManager::Init() noexcept {
  world_.Init(/*Math::Vec2F(0.f, 5.81f)*/);

  player_controller_.Init();
}

void GameManager::PollInputs() noexcept { player_controller_.PollInputs(); }

void GameManager::Update() noexcept {
  player_controller_.Update();

  world_.Update(game_constants::kFixedDeltaTime);
}

void GameManager::Deinit() noexcept {
  world_.Deinit();
}
