#include "game_manager.h"

void GameManager::Init(int local_player_id) noexcept {
  world_.Init(Math::Vec2F(0.f, 5.81f));
  world_.SetContactListener(this);

  //for (std::size_t i = 0; i < game_constants::kMaxPlayerCount; i++) {
  //  player_controllers_[i].RegisterWorld(&world_);
  //  player_controllers_[i].Init();
  //}

  player_manager_.RegisterWorld(&world_);
  player_manager_.Init();

  platform_manager_.Init(&world_);

  local_player_id_ = local_player_id;
}

void GameManager::PollInputs() noexcept {
  //for (auto& player_controller : player_controllers_) {
  //  player_controller.PollInputs();
  //}

  //player_manager_.PollInputs();
}

void GameManager::Update() noexcept {
  //PollInputs();

  const auto input = inputs::GetPlayerInput(local_player_id_);
  player_manager_.SetPlayerInput(input, local_player_id_);
  //for (auto& player_controller : player_controllers_) {
  //  player_controller.Update();
  //}

  player_manager_.Update();

  world_.Update(game_constants::kFixedDeltaTime);
}

void GameManager::Deinit() noexcept {
  world_.Deinit();
}

void GameManager::SetPlayerInput(inputs::PlayerInput input, std::size_t idx) {
  player_manager_.SetPlayerInput(input, idx);
}

void GameManager::OnTriggerEnter(PhysicsEngine::ColliderRef colliderRefA,
                                 PhysicsEngine::ColliderRef colliderRefB) noexcept {
  player_manager_.OnTriggerEnter(colliderRefA, colliderRefB);
}

void GameManager::OnTriggerStay(PhysicsEngine::ColliderRef colliderRefA,
  PhysicsEngine::ColliderRef colliderRefB) noexcept {

}

void GameManager::OnTriggerExit(PhysicsEngine::ColliderRef colliderRefA,
  PhysicsEngine::ColliderRef colliderRefB) noexcept {
}
