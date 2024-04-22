#include "game_manager.h"

#include "rollback_manager.h"

void GameManager::Init(int local_player_id) noexcept {
  game_.world.Init(Math::Vec2F(0.f, 0.f), 110);
  game_.world.SetContactListener(this);

  player_manager_.RegisterWorld(&game_.world);
  player_manager_.RegisterProjectileManager(&projectile_manager_);
  player_manager_.Init();

  projectile_manager_.Init(&game_.world);
  platform_manager_.Init(&game_.world);

  local_player_id_ = local_player_id;
}

void GameManager::FixedUpdate(FrameNbr frame_nbr) noexcept {
  for (PlayerId id = 0; id < game_constants::kMaxPlayerCount; id++) {
    const auto input = rollback_manager_->GetPlayerInputAtFrame(id, frame_nbr);
    player_manager_.SetPlayerInput(input, id);
  }

  player_manager_.FixedUpdate();

  game_.world.Update(game_constants::kFixedDeltaTime);
}

void GameManager::Deinit() noexcept {
  game_.world.Deinit();
}

void GameManager::Copy(const GameManager& game_manager) noexcept {
  game_.world = game_manager.game_.world;
  player_manager_.Copy(game_manager.player_manager_);
  projectile_manager_.Copy(game_manager.projectile_manager_);
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
