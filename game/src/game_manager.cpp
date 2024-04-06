#include "game_manager.h"

void GameManager::Init() noexcept {
  world_.Init(/*Math::Vec2F(0.f, 5.81f)*/);

  for (std::size_t i = 0; i < game_constants::kMaxPlayerCount; i++) {
    player_controllers_[i].RegisterWorld(&world_);
    player_controllers_[i].Init();
  }
}

void GameManager::PollInputs() noexcept {
  for (auto& player_controller : player_controllers_) {
    player_controller.PollInputs();
  }
}

void GameManager::Update() noexcept {
  PollInputs();

  for (auto& player_controller : player_controllers_) {
    player_controller.Update();
  }

  world_.Update(game_constants::kFixedDeltaTime);
}

void GameManager::Deinit() noexcept {
  world_.Deinit();
}
