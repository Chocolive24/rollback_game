#include "player_manager.h"
#include "raylib_wrapper.h"
#include "Metrics.h"

#include <cassert>

void PlayerManager::Init() noexcept {
  for (std::size_t i = 0; i < game_constants::kMaxPlayerCount; i++) {
    Math::Vec2F start_pos{};
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
    body.SetDamping(0.3f);
    const auto main_col_ref = world_->CreateCollider(body_ref);
    auto& collider = world_->GetCollider(main_col_ref);

    constexpr auto half_size =
        Math::Vec2F(game_constants::kPlayerMainColLength * 0.5f,
                    game_constants::kPlayerMainColLength * 0.5f);
    collider.SetShape(Math::RectangleF(-half_size, half_size));
    collider.SetRestitution(0.3f);

    const auto jump_col_ref = world_->CreateCollider(body_ref);
    auto& can_jump_col = world_->GetCollider(jump_col_ref);

    can_jump_col.SetShape(Math::CircleF(Math::Vec2F(0.f, 0.f), 0.1f));
    can_jump_col.SetOffset(game_constants::kPlayerJumpColOffset);
    can_jump_col.SetIsTrigger(true);

    players_[i].main_col_ref = main_col_ref;
    players_[i].jump_col_ref = jump_col_ref;
  }
}

void PlayerManager::FixedUpdate() noexcept {
  for (auto& player : players_) {
    Move(player);

    if (player.shoot_timer_ > Math::Epsilon) {
      player.shoot_timer_ -= game_constants::kFixedDeltaTime;

      if (player.shoot_timer_ <= Math::Epsilon)
      {
        player.shoot_timer_ = 0.f;
      }

      return;
    }

    Shoot(player);
    player.shoot_timer_ = kShootCooldown;
  }
}

void PlayerManager::Move(const Player& player) const noexcept {
  auto move_direction = Math::Vec2F::Zero();

  if (player.input &
      static_cast<inputs::PlayerInput>(inputs::PlayerInputType::kUp)) {
    move_direction += Math::Vec2F::Down();
  }
  if (player.input &
      static_cast<inputs::PlayerInput>(inputs::PlayerInputType::kLeft)) {
    move_direction += Math::Vec2F::Left();
  }
  if (player.input &
      static_cast<inputs::PlayerInput>(inputs::PlayerInputType::kDown)) {
    move_direction += Math::Vec2F::Up();
  }
  if (player.input &
      static_cast<inputs::PlayerInput>(inputs::PlayerInputType::kRight)) {
    move_direction += Math::Vec2F::Right();
  }

  if (move_direction.Length() >= Math::Epsilon) {
    const auto& body_ref =
        world_->GetCollider(player.main_col_ref).GetBodyRef();
    auto& body = world_->GetBody(body_ref);
    const auto dir = move_direction.Length() >= Math::Epsilon
                         ? move_direction.Normalized()
                         : move_direction;
    const auto val = dir * game_constants::kPlayerSpeedMoveFactor;
    body.ApplyForce(val);
  }
}


void PlayerManager::Shoot(const Player& player) const noexcept {
  assert(projectile_manager_ != nullptr,
         "No projectiles manager pointer given to player manager.\n");

    // TODO: pas de creation de proj si le click s'est fait dans le joueur.
   if (player.input &
      static_cast<inputs::PlayerInput>(inputs::PlayerInputType::kShoot)) {
    
     const auto& body =
         world_->GetBody(world_->GetCollider(player.main_col_ref).GetBodyRef());
     //const auto proj_v = mouse_pos - body.Position();
     //const auto proj_dir = proj_v.Normalized();

     projectile_manager_->CreateProjectile(body.Position() + Math::Vec2F(0.5f, 0.f),
                                           Math::Vec2F::Right());
   }
}

void PlayerManager::Copy(const PlayerManager& player_manager) noexcept {
  players_ = player_manager.players_;
  //for (std::size_t i = 0; i < game_constants::kMaxPlayerCount; i++) {
  //  players_[i] = player_manager.players_[i];
  //}
}

// Function to compute checksum for the players state.
int PlayerManager::ComputeChecksum() const noexcept {
  int checksum = 0;

  for (const auto& player : players_) {
    const auto& body_ref = world_->GetCollider(player.main_col_ref).GetBodyRef();
    const auto& body = world_->GetBody(body_ref);

    const auto& pos = body.Position();
    const auto* pos_ptr = reinterpret_cast<const int*>(&pos);

    // Add position
    for (size_t i = 0; i < sizeof(Math::Vec2F) / sizeof(int); i++) {
      checksum += pos_ptr[i];
    }

    // Add velocity
    const auto& velocity = body.Velocity();
    const auto* velocity_ptr = reinterpret_cast<const int*>(&velocity);
    for (size_t i = 0; i < sizeof(Math::Vec2F) / sizeof(int); i++) {
      checksum += velocity_ptr[i];
    }

    // Add forces
    //const auto& forces = body.Forces();
    //const auto* forces_ptr = reinterpret_cast<const int*>(&forces);
    //for (size_t i = 0; i < sizeof(Math::Vec2F) / sizeof(int); i++) {
    //  checksum += forces_ptr[i];
    //}

    // Add input.
    checksum += static_cast<int>(player.input);
  }

  return checksum;
}

void PlayerManager::OnTriggerEnter(
    PhysicsEngine::ColliderRef colliderRefA,
    PhysicsEngine::ColliderRef colliderRefB) noexcept {
  // if (colliderRefA == jump_col_ref_ || colliderRefB == jump_col_ref_) {
  //   if (colliderRefA == main_col_ref_ || colliderRefB == main_col_ref_) {
  //     return;
  //   }

  //  can_jump_ = true;
  //  is_jumping_ = false;
  //}

  // if (colliderRefA == platforms_manager_.platform_col_refs[0] ||
  //     colliderRefB == platforms_manager_.platform_col_refs[0]) {
  //   if (colliderRefA == player_controller_.jump_col_ref_ ||
  //       colliderRefB == player_controller_.jump_col_ref_) {
  //     std::cout << "trigger enter \n";
  //     player_controller_.can_jump_ = true;
  //     player_controller_.is_jumping_ = false;
  //   }
  // }
}

void PlayerManager::SetPlayerInput(inputs::PlayerInput input, PlayerId player_id) {
  players_[player_id].input = input;
}

Math::Vec2F PlayerManager::GetPlayerPosition(std::size_t idx) const noexcept {
  const auto& body_ref =
      world_->GetCollider(players_[idx].main_col_ref).GetBodyRef();
   const auto& body = world_->GetBody(body_ref);
   return body.Position();
}

Math::Vec2F PlayerManager::GetJumpColliderPosition(
    std::size_t idx) const noexcept {
   const auto& body_ref =
       world_->GetCollider(players_[idx].main_col_ref).GetBodyRef();
   const auto& body = world_->GetBody(body_ref);
   return body.Position() + game_constants::kPlayerJumpColOffset;
}

Math::CircleF PlayerManager::GetJumpColliderShape(
    std::size_t idx) const noexcept {
   const auto& col = world_->GetCollider(players_[idx].jump_col_ref);
   return std::get<Math::CircleF>(col.Shape());
}
