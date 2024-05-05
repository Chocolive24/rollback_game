#include "player_manager.h"
#include "Metrics.h"

void PlayerManager::Init() noexcept {
  for (std::size_t i = 0; i < game_constants::kMaxPlayerCount; i++) {
    Math::Vec2F start_pos;
    switch (i) {
      case 0:
        start_pos = game_constants::kPlayer1StartPos;
        break;
      case 1:
        start_pos = game_constants::kPlayer2StartPos;
        break;
      default:
        start_pos = Math::Vec2F::Zero();
        break;
    }

    const auto& body_ref = world_->CreateBody();
    auto& body = world_->GetBody(body_ref);
    body.SetPosition(start_pos);
    body.SetDamping(kPlayerDamping);
    body.SetMass(kPlayerMass);
    const auto main_col_ref = world_->CreateCollider(body_ref);
    auto& collider = world_->GetCollider(main_col_ref);

    constexpr auto half_size =
        Math::Vec2F(game_constants::kPlayerMainColLength * 0.5f,
                    game_constants::kPlayerMainColLength * 0.5f);
    collider.SetShape(Math::CircleF(Math::Vec2F::Zero(), half_size.X));
    collider.SetRestitution(0.3f);

    players_[i].main_col_ref = main_col_ref;
  }
}

void PlayerManager::FixedUpdate() noexcept {
  for (auto& player : players_) {
    Move(player);

    if (player.spin_timer > Math::Epsilon) {
      player.spin_timer -= game_constants::kFixedDeltaTime;

      if (player.spin_timer <= Math::Epsilon) {
        player.spin_timer = 0.f;
      }
    }

    if (player.damage_timer > Math::Epsilon) {
      player.damage_timer -= game_constants::kFixedDeltaTime;

      if (player.damage_timer <= Math::Epsilon) {
        player.damage_timer = 0.f;
      }
    }

    if (player.shoot_timer > Math::Epsilon) {
      player.shoot_timer -= game_constants::kFixedDeltaTime;

      if (player.shoot_timer <= Math::Epsilon)
      {
        player.shoot_timer = 0.f;
      }

      continue;
    }

    if (player.input &
        static_cast<input::PlayerInput>(input::PlayerInputType::kShoot)) {
      const auto& body = world_->GetBody(
          world_->GetCollider(player.main_col_ref).GetBodyRef());

      const auto proj_pos = body.Position() + player.dir_to_mouse * 0.75f;
      projectile_manager_->CreateProjectile(proj_pos, player.dir_to_mouse);

      player.shoot_timer = kShootCooldown;
    }
  }
}

void PlayerManager::Move(const Player& player) const noexcept {
  auto move_direction = Math::Vec2F::Zero();

  if (player.input &
      static_cast<input::PlayerInput>(input::PlayerInputType::kUp)) {
    move_direction += Math::Vec2F::Down();
  }
  if (player.input &
      static_cast<input::PlayerInput>(input::PlayerInputType::kLeft)) {
    move_direction += Math::Vec2F::Left();
  }
  if (player.input &
      static_cast<input::PlayerInput>(input::PlayerInputType::kDown)) {
    move_direction += Math::Vec2F::Up();
  }
  if (player.input &
      static_cast<input::PlayerInput>(input::PlayerInputType::kRight)) {
    move_direction += Math::Vec2F::Right();
  }


  if (move_direction.Length() >= Math::Epsilon) {
    
        const auto& body_ref =
        world_->GetCollider(player.main_col_ref).GetBodyRef();
    auto& body = world_->GetBody(body_ref);

    //std::cout << body.Velocity().Length() << '\n';
 
    if (body.Velocity().Length() >= 8.f)
    {
      return;
    }

    const auto dir = move_direction.Length() >= Math::Epsilon
                         ? move_direction.Normalized()
                         : move_direction;
    const auto val = dir * game_constants::kPlayerSpeedMoveFactor;
    body.ApplyForce(val);
  }
}

void PlayerManager::Rollback(const PlayerManager& player_manager) noexcept {
  players_ = player_manager.players_;
}

// Function to compute checksum for the players state.
Checksum PlayerManager::ComputeChecksum() const noexcept {
  Checksum checksum = 0;

  for (const auto& player : players_) {
    const auto& body_ref = world_->GetCollider(player.main_col_ref).GetBodyRef();
    const auto& body = world_->GetBody(body_ref);

    const auto& pos = body.Position();
    const auto* pos_ptr = reinterpret_cast<const Checksum*>(&pos);

    // Add position
    for (size_t i = 0; i < sizeof(Math::Vec2F) / sizeof(Checksum); i++) {
      checksum += pos_ptr[i];
    }

    // Add velocity
    const auto& velocity = body.Velocity();
    const auto* velocity_ptr = reinterpret_cast<const Checksum*>(&velocity);
    for (size_t i = 0; i < sizeof(Math::Vec2F) / sizeof(Checksum); i++) {
      checksum += velocity_ptr[i];
    }

    // Add input.
    checksum += player.input;

    // Add shoot_timer.
    const auto* shoot_timer_ptr = reinterpret_cast<const Checksum*>(&player.shoot_timer);
    checksum += *shoot_timer_ptr;

    // Add hp.
    checksum += player.hp;

    // Add damage_timer.
    const auto* damage_timer_ptr = reinterpret_cast<const Checksum*>(&player.damage_timer);
    checksum += *damage_timer_ptr;
  }

  return checksum;
}

void PlayerManager::SetPlayerInput(const input::FrameInput& input, PlayerId player_id) {
  players_[player_id].input = input.input();
  players_[player_id].dir_to_mouse = input.dir_to_mouse();
}

void PlayerManager::ApplyOneDamageToPlayer(std::size_t player_idx) noexcept {
  auto& player = players_[player_idx];

  if (player.damage_timer <= Math::Epsilon) {
    player.hp--;
    player.damage_timer = kDamageCooldown;
  }
}

std::int8_t PlayerManager::GetPlayerHp(std::size_t idx) const noexcept {
  return players_[idx].hp;
}

Math::Vec2F PlayerManager::GetPlayerPosition(std::size_t idx) const noexcept {
  const auto& body_ref =
      world_->GetCollider(players_[idx].main_col_ref).GetBodyRef();
   const auto& body = world_->GetBody(body_ref);
   return body.Position();
}

Math::Vec2F PlayerManager::GetPlayerVelocity(std::size_t idx) const noexcept {
   const auto& body_ref =
       world_->GetCollider(players_[idx].main_col_ref).GetBodyRef();
   const auto& body = world_->GetBody(body_ref);
   return body.Velocity();
}

PhysicsEngine::ColliderRef PlayerManager::GetPlayerColRef(std::size_t idx) const noexcept {
   return players_[idx].main_col_ref;
}

Math::Vec2F PlayerManager::GetJumpColliderPosition(
    std::size_t idx) const noexcept {
   const auto& body_ref =
       world_->GetCollider(players_[idx].main_col_ref).GetBodyRef();
   const auto& body = world_->GetBody(body_ref);
   return body.Position() + game_constants::kPlayerJumpColOffset;
}

Math::CircleF PlayerManager::GetJumpColliderShape(std::size_t idx) const noexcept {
   const auto& col = world_->GetCollider(players_[idx].jump_col_ref);
   return std::get<Math::CircleF>(col.Shape());
}

bool PlayerManager::IsPlayerWalking(std::size_t idx) const noexcept {
   return players_[idx].input != 0 && players_[idx].input != 
       static_cast<int>(input::PlayerInputType::kShoot);
}

bool PlayerManager::IsPlayerSpinning(std::size_t idx) const noexcept {
   return players_[idx].spin_timer > Math::Epsilon;
}

bool PlayerManager::IsPlayerHurt(std::size_t idx) const noexcept {
   return players_[idx].damage_timer > Math::Epsilon;
}

bool PlayerManager::IsPlayerFacingRight(std::size_t idx) const noexcept {
   if (players_[idx].input & static_cast<int>(input::PlayerInputType::kRight)) {
       return true;
   }

  if (players_[idx].input & static_cast<int>(input::PlayerInputType::kLeft)) {
    return false;
   }

   const auto& body_ref =
       world_->GetCollider(players_[idx].main_col_ref).GetBodyRef();
   const auto& body = world_->GetBody(body_ref);

   return body.Velocity().X > Math::Epsilon;
}

void PlayerManager::LaunchSpinTimer(std::size_t idx) noexcept {
   players_[idx].spin_timer = kSpinCooldown;
}
