#include "game_manager.h"
#include "rollback_manager.h"
#include "raylib_wrapper.h"

void GameManager::Init(PlayerId player_id, int input_profile_id) noexcept {
  world_.Init(Math::Vec2F(0.f, 0.f), 110);
  world_.SetContactListener(this);

  player_manager_.RegisterWorld(&world_);
  player_manager_.RegisterProjectileManager(&projectile_manager_);
  player_manager_.Init();

  projectile_manager_.Init(&world_);
  platform_manager_.Init(&world_);

  player_id_ = player_id;
  input_profile_id_ = input_profile_id;
}

void GameManager::Update() noexcept {
  //fixed_timer_ += raylib::GetFrameTime();
  //while (fixed_timer_ >= game_constants::kFixedDeltaTime) {
  //  current_frame_++;
  //  FixedUpdate(current_frame_);
  //  fixed_timer_ -= game_constants::kFixedDeltaTime;
  //}
}

void GameManager::FixedUpdate(FrameNbr frame_nbr) noexcept {
  for (PlayerId id = 0; id < game_constants::kMaxPlayerCount; id++) {
    const auto input = rollback_manager_->GetPlayerInputAtFrame(id, frame_nbr);
    player_manager_.SetPlayerInput(input, id);
  }

  player_manager_.FixedUpdate();

  world_.Update(game_constants::kFixedDeltaTime);
}

void GameManager::Deinit() noexcept {
  world_.Deinit();
}

void GameManager::Copy(const GameManager& game_manager) noexcept {
  world_ = game_manager.world_;
  player_manager_.Copy(game_manager.player_manager_);
  projectile_manager_.Copy(game_manager.projectile_manager_);
}

int GameManager::ComputeChecksum() const noexcept {
  int checksum = 0;

  checksum += player_manager_.ComputeChecksum();

  return checksum;
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
