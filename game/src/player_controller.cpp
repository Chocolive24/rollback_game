#include "player_controller.h"
#include "game_constants.h"
#include "game_renderer.h"
#include "inputs.h"

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
    collider.SetRestitution(0.f);

    const auto jump_col_ref = world_->CreateCollider(body_ref);
    auto& can_jump_col = world_->GetCollider(jump_col_ref);

    can_jump_col.SetShape(Math::CircleF(Math::Vec2F(0.f, 0.f), 0.1f));
    can_jump_col.SetOffset(game_constants::kPlayerJumpColOffset);
    can_jump_col.SetIsTrigger(true);

    players_[i].id = i;
    players_[i].main_col_ref = main_col_ref;
    players_[i].jump_col_ref = jump_col_ref;
  }
}

void PlayerManager::FixedUpdate() noexcept {
  for (const auto& player : players_) {
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
      const auto& body_ref = world_->GetCollider(player.main_col_ref).GetBodyRef();
      auto& body = world_->GetBody(body_ref);
      const auto dir = move_direction.Length() >= Math::Epsilon
                           ? move_direction.Normalized()
                           : move_direction;
      const auto val = dir * game_constants::kPlayerSpeedMoveFactor;
      body.ApplyForce(val);
    }

    //player.position += move_direction * 5.f * game_constants::kFixedDeltaTime;
  }
}

void PlayerManager::PollInputs() noexcept {
  //const auto inputs = inputs::GetPlayerInput(1);
  //move_direction = Math::Vec2F::Zero();

  //if (inputs & static_cast<inputs::PlayerInput>(inputs::PlayerInputType::kUp)) {
  //  move_direction += Math::Vec2F::Down();
  //}
  //if (inputs &
  //    static_cast<inputs::PlayerInput>(inputs::PlayerInputType::kLeft)) {
  //  move_direction += Math::Vec2F::Left();
  //}
  //if (inputs &
  //    static_cast<inputs::PlayerInput>(inputs::PlayerInputType::kDown)) {
  //  move_direction += Math::Vec2F::Up();
  //}
  //if (inputs &
  //    static_cast<inputs::PlayerInput>(inputs::PlayerInputType::kRight)) {
  //  move_direction += Math::Vec2F::Right();
  //}

  // if (inputs &
  // static_cast<inputs::PlayerInput>(inputs::PlayerInputType::kJump)) {
  //   if (is_jumping_) {
  //     return;
  //   }

  //  is_jumping_ = true;
  //  can_jump_ = false;
  //  const auto& body_ref = world_->GetCollider(main_col_ref_).GetBodyRef();
  //  auto& body = world_->GetBody(body_ref);

  //  body.ApplyForce(Math::Vec2F::Down() *
  //  game_constants::kPlayerJumpMagnitude);
  //}

  // if (inputs &
  // static_cast<inputs::PlayerInput>(inputs::PlayerInputType::kShoot)) {
  //  Shoot();
  //}
}

void PlayerManager::Shoot() noexcept {
  // const auto& body_ref = world_->GetCollider(main_col_ref_).GetBodyRef();
  // const auto& body = world_->GetBody(body_ref);

  // const auto& proj_body_ref = world_->CreateBody();
  // auto& proj_body = world_->GetBody(proj_body_ref);
  // proj_body.SetPosition(body.Position() + Math::Vec2F(0.5f, 0.f));

  // const auto& col_ref = world_->CreateCollider(proj_body_ref);
  // auto& collider = world_->GetCollider(col_ref);

  // collider.SetShape(Math::CircleF(Math::Vec2F::Zero(), 0.1f));
  // collider.SetRestitution(0.f);
}

void PlayerManager::Copy(const PlayerManager& player_manager) noexcept {
  for (std::size_t i = 0; i < game_constants::kMaxPlayerCount; i++) {
    players_[i] = player_manager.players_[i];
  }
}

// Function to compute checksum for the players state.
int PlayerManager::ComputeChecksum() const noexcept {
  int checksum = 0;

  for (const auto& player : players_) {
    const auto& body_ref = world_->GetCollider(player.main_col_ref).GetBodyRef();
    const auto& body = world_->GetBody(body_ref);

    const auto& pos = body.Position();
    const auto* posPtr = reinterpret_cast<const int*>(&pos);

    // Add position
    for (size_t i = 0; i < sizeof(Math::Vec2F) / sizeof(int); i++) {
      checksum += posPtr[i];
    }

    // Add velocity
    const auto& velocity = body.Velocity();
    const auto* velocityPtr = reinterpret_cast<const int*>(&velocity);
    for (size_t i = 0; i < sizeof(Math::Vec2F) / sizeof(int); i++) {
      checksum += velocityPtr[i];
    }

    // Add forces
    const auto& forces = body.Forces();
    const auto* forces_ptr = reinterpret_cast<const int*>(&forces);
    for (size_t i = 0; i < sizeof(Math::Vec2F) / sizeof(int); i++) {
      checksum += forces_ptr[i];
    }

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
  //return players_[idx].position;
}

Math::Vec2F PlayerManager::GetJumpColliderPosition(
    std::size_t idx) const noexcept {
   const auto& body_ref =
       world_->GetCollider(players_[idx].main_col_ref).GetBodyRef();
   const auto& body = world_->GetBody(body_ref);
   return body.Position() + game_constants::kPlayerJumpColOffset;
  //return Math::Vec2F(-1000, -1000);
}

Math::CircleF PlayerManager::GetJumpColliderShape(
    std::size_t idx) const noexcept {
   const auto& col = world_->GetCollider(players_[idx].jump_col_ref);
   return std::get<Math::CircleF>(col.Shape());
  //return Math::CircleF(Math::Vec2F(0.f, 0.f), 0.f);
}
