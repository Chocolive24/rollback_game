#include "game_manager.h"
#include "rollback_manager.h"
#include "raylib_wrapper.h"

void GameManager::Init(PlayerId player_id, int input_profile_id) noexcept {
  game_state_.world.Init(Math::Vec2F(0.f, 0.f), 110);
  game_state_.world.SetContactListener(this);

  game_state_.player_manager.RegisterWorld(&game_state_.world);
  game_state_.player_manager.RegisterProjectileManager(&game_state_.projectile_manager);
  game_state_.player_manager.Init();

  game_state_.projectile_manager.Init(&game_state_.world);
  platform_manager_.Init(&game_state_.world);

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
    game_state_.player_manager.SetPlayerInput(input, id);
  }

  game_state_.player_manager.FixedUpdate();

  game_state_.world.Update(game_constants::kFixedDeltaTime);
}

void GameManager::Deinit() noexcept {
  game_state_.world.Deinit();
}

void GameManager::Copy(const GameManager& game_manager) noexcept {
  game_state_.world = game_manager.game_state_.world;
  game_state_.player_manager.Copy(game_manager.game_state_.player_manager);
  game_state_.projectile_manager.Copy(game_manager.game_state_.projectile_manager);
}

Checksum GameManager::ComputeChecksum() const noexcept {
  Checksum checksum = 0;

  checksum += game_state_.player_manager.ComputeChecksum();

  return checksum;
}

void GameManager::OnTriggerEnter(PhysicsEngine::ColliderRef colliderRefA,
                                 PhysicsEngine::ColliderRef colliderRefB) noexcept {
  game_state_.player_manager.OnTriggerEnter(colliderRefA, colliderRefB);
}

void GameManager::OnTriggerStay(PhysicsEngine::ColliderRef colliderRefA,
  PhysicsEngine::ColliderRef colliderRefB) noexcept {

}

void GameManager::OnTriggerExit(PhysicsEngine::ColliderRef colliderRefA,
  PhysicsEngine::ColliderRef colliderRefB) noexcept {
}
