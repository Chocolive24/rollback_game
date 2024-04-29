#include "local_game_manager.h"

#ifdef TRACY_ENABLE
#include <Tracy.hpp>
#endif

void LocalGameManager::Init(int input_profile_id) noexcept {
  game_state_.world.Init(Math::Vec2F(0.f, 0.f), game_constants::kGameBodyCount);
  game_state_.world.SetContactListener(this);

  game_state_.player_manager.RegisterWorld(&game_state_.world);
  game_state_.player_manager.RegisterProjectileManager(&game_state_.projectile_manager);
  game_state_.player_manager.Init();

  game_state_.projectile_manager.Init(&game_state_.world);
  platform_manager_.Init(&game_state_.world);

  input_profile_id_ = input_profile_id;
}

void LocalGameManager::FixedUpdate() noexcept {
#ifdef TRACY_ENABLE
    ZoneScoped;
#endif  // TRACY_ENABLE

  game_state_.world.Update(game_constants::kFixedDeltaTime);

  game_state_.player_manager.FixedUpdate();
  game_state_.projectile_manager.FixedUpdate();
}

void LocalGameManager::Deinit() noexcept {
  game_state_.world.Deinit();
}

void LocalGameManager::SetPlayerInput(const input::FrameInput& input, PlayerId player_id) noexcept {
  game_state_.player_manager.SetPlayerInput(input, player_id);
}

void LocalGameManager::Rollback(const LocalGameManager& game_manager) noexcept {
  game_state_.world = game_manager.game_state_.world;
  game_state_.world.SetContactListener(this);
  game_state_.player_manager.Rollback(game_manager.game_state_.player_manager);
  game_state_.projectile_manager.Rollback(game_manager.game_state_.projectile_manager);
}

Checksum LocalGameManager::ComputeChecksum() const noexcept {
  Checksum checksum = 0;

  checksum += game_state_.player_manager.ComputeChecksum();
  checksum += game_state_.projectile_manager.ComputeChecksum();

  return checksum;
}

void LocalGameManager::OnCollisionEnter(
    PhysicsEngine::ColliderRef colliderRefA,
    PhysicsEngine::ColliderRef colliderRefB) noexcept {

  for (std::size_t player_idx = 0; player_idx < game_constants::kMaxPlayerCount; player_idx++) {
    const auto& player_col_ref = game_state_.player_manager.GetPlayerColRef(player_idx);

    if (colliderRefA == player_col_ref || colliderRefB == player_col_ref)
    {
      for (std::size_t wall_idx = 0; wall_idx < game_constants::kArenaWallCount; wall_idx++)
      {
        const auto& wall_col_ref = platform_manager_.GetWallColRef(wall_idx);
        if (colliderRefA == wall_col_ref || colliderRefB == wall_col_ref)
        {
          game_state_.player_manager.ApplyOneDamageToPlayer(player_idx);
        }
      }
    }
  }

  //game_state_.player_manager.OnCollisionEnter(colliderRefA, colliderRefB);
  game_state_.projectile_manager.OnCollisionEnter(colliderRefA, colliderRefB);
}
