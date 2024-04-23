#include "game_manager.h"
#include "rollback_manager.h"
#include "raylib_wrapper.h"

void GameManager::Init(PlayerId player_id, int input_profile_id) noexcept {
  game_.world.Init(Math::Vec2F(0.f, 0.f), 110);
  game_.world.SetContactListener(this);

  player_manager_.RegisterWorld(&game_.world);
  player_manager_.RegisterProjectileManager(&projectile_manager_);
  player_manager_.Init();

  projectile_manager_.Init(&game_.world);
  platform_manager_.Init(&game_.world);

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

  game_.world.Update(game_constants::kFixedDeltaTime);
}

void GameManager::Deinit() noexcept {
  game_.world.Deinit();
}

void GameManager::Copy(const GameManager& game_manager) noexcept {
  game_ = game_manager.game_;
  //game_.world = game_manager.game_.world;
  //player_manager_.Copy(game_manager.player_manager_);
  //projectile_manager_.Copy(game_manager.projectile_manager_);
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
