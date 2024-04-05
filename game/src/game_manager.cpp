#include "game_manager.h"

void GameManager::Init() noexcept {
  world_.Init(/*Math::Vec2F(0.f, 5.81f)*/);

  player_controller_.Init();
}

void GameManager::PollInputs() noexcept {
  player_controller_.PollInputs();
}

void GameManager::Update() noexcept {
  PollInputs();

  player_controller_.Update();

  world_.Update(game_constants::kFixedDeltaTime);
}

void GameManager::Deinit() noexcept {
  world_.Deinit();
}
