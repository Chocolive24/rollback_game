#include "game_manager.h"

void GameManager::Init(int local_player_id) noexcept {
  world_.Init(Math::Vec2F(0.f, 5.81f), 5);
  world_.SetContactListener(this);

  player_manager_.RegisterWorld(&world_);
  player_manager_.Init();

  platform_manager_.Init(&world_);

  local_player_id_ = local_player_id;
}

void GameManager::FixedUpdate() noexcept {
  player_manager_.FixedUpdate();

  world_.Update(game_constants::kFixedDeltaTime);
}

void GameManager::Deinit() noexcept {
  world_.Deinit();
}

void GameManager::Copy(const GameManager& game_manager) noexcept {
  //TODO: copy game state data.
  player_manager_.Copy(game_manager.player_manager_);
}

int GameManager::ComputeChecksum() const noexcept {
  int checksum = 0;

  checksum += player_manager_.ComputeChecksum();

  return checksum;
}

void GameManager::SetPlayerInput(inputs::PlayerInput input, PlayerId player_id) {
  player_manager_.SetPlayerInput(input, player_id);
}

void GameManager::SetRemotePlayerInput(inputs::FrameInput input, PlayerId player_id) {
  //rollback_manager_.SetRemotePlayerInput(input, player_id);
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
