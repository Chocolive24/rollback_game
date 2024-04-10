#include "game_manager.h"

#include <iostream>

void GameManager::Init() noexcept {
  world_.Init(Math::Vec2F(0.f, 5.81f));
  world_.SetContactListener(this);

  //for (std::size_t i = 0; i < game_constants::kMaxPlayerCount; i++) {
  //  player_controllers_[i].RegisterWorld(&world_);
  //  player_controllers_[i].Init();
  //}

  player_controller_.RegisterWorld(&world_);
  player_controller_.Init();

  platforms_manager_.Init(&world_);
}

void GameManager::PollInputs() noexcept {
  //for (auto& player_controller : player_controllers_) {
  //  player_controller.PollInputs();
  //}

  player_controller_.PollInputs();
}

void GameManager::Update() noexcept {
  PollInputs();

  //for (auto& player_controller : player_controllers_) {
  //  player_controller.Update();
  //}

  player_controller_.Update();

  world_.Update(game_constants::kFixedDeltaTime);
}

void GameManager::Deinit() noexcept {
  world_.Deinit();
}

void GameManager::OnTriggerEnter(PhysicsEngine::ColliderRef colliderRefA,
  PhysicsEngine::ColliderRef colliderRefB) noexcept {
  if (colliderRefA == platforms_manager_.platform_col_refs[0] ||
      colliderRefB == platforms_manager_.platform_col_refs[0]) {

    if (colliderRefA == player_controller_.can_jump_col_ref_ ||
        colliderRefB == player_controller_.can_jump_col_ref_) {
      std::cout << "trigger enter \n";
      player_controller_.can_jump_ = true;
      player_controller_.is_jumping_ = false;
    }
  }
}

void GameManager::OnTriggerStay(PhysicsEngine::ColliderRef colliderRefA,
  PhysicsEngine::ColliderRef colliderRefB) noexcept {

}

void GameManager::OnTriggerExit(PhysicsEngine::ColliderRef colliderRefA,
  PhysicsEngine::ColliderRef colliderRefB) noexcept {
  std::cout << "trigger exit\n";
}
