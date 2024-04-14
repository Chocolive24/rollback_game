#include "player_controller.h"

#include <iostream>

#include "game_constants.h"
#include "inputs.h"

void PlayerController::Init() noexcept {
  const auto& body_ref = world_->CreateBody();
  auto& body = world_->GetBody(body_ref);
  body.SetPosition(game_constants::kPlayerStartPos);
  body.SetDamping(0.3f);
  main_col_ref_ = world_->CreateCollider(body_ref);
  auto& collider = world_->GetCollider(main_col_ref_);

  constexpr auto half_size = Math::Vec2F(game_constants::kPlayerMainColLength * 0.5f,
                                     game_constants::kPlayerMainColLength * 0.5f);
  collider.SetShape(Math::RectangleF(-half_size, half_size));
  collider.SetRestitution(0.f);

  jump_col_ref_ = world_->CreateCollider(body_ref);
  auto& can_jump_col = world_->GetCollider(jump_col_ref_);

  can_jump_col.SetShape(Math::CircleF(Math::Vec2F(0.f, 0.f), 0.1f));
  can_jump_col.SetOffset(game_constants::kPlayerJumpColOffset);
  can_jump_col.SetIsTrigger(true);
}

void PlayerController::Update() const noexcept {
  if (move_direction_.Length() >= Math::Epsilon) {
    const auto& body_ref = world_->GetCollider(main_col_ref_).GetBodyRef();
    auto& body = world_->GetBody(body_ref);
    const auto dir = move_direction_.Length() >= Math::Epsilon
                         ? move_direction_.Normalized()
                         : move_direction_;
    const auto val = dir * game_constants::kPlayerSpeedMoveFactor;
    body.ApplyForce(val);
  }
}

void PlayerController::PollInputs() noexcept {
  const auto inputs = inputs::GetPlayerInput(1);
  move_direction_ = Math::Vec2F::Zero();

  if (inputs & static_cast<inputs::PlayerInput>(inputs::PlayerInputType::kUp)) {
    move_direction_ += Math::Vec2F::Down();
  }
  if (inputs & static_cast<inputs::PlayerInput>(inputs::PlayerInputType::kLeft)) {
    move_direction_ += Math::Vec2F::Left();
  }
  if (inputs & static_cast<inputs::PlayerInput>(inputs::PlayerInputType::kDown)) {
    move_direction_ += Math::Vec2F::Up();
  }
  if (inputs & static_cast<inputs::PlayerInput>(inputs::PlayerInputType::kRight)) {
    move_direction_ += Math::Vec2F::Right();
  }

  if (inputs & static_cast<inputs::PlayerInput>(inputs::PlayerInputType::kJump)) {
    if (is_jumping_) {
      return;
    }

    is_jumping_ = true;
    can_jump_ = false;
    const auto& body_ref = world_->GetCollider(main_col_ref_).GetBodyRef();
    auto& body = world_->GetBody(body_ref);

    body.ApplyForce(Math::Vec2F::Down() * game_constants::kPlayerJumpMagnitude);
  }

   if (inputs & static_cast<inputs::PlayerInput>(inputs::PlayerInputType::kShoot)) {
    Shoot();
  }
}

void PlayerController::Shoot() noexcept {
  //const auto& body_ref = world_->GetCollider(main_col_ref_).GetBodyRef();
  //const auto& body = world_->GetBody(body_ref);

  //const auto& proj_body_ref = world_->CreateBody();
  //auto& proj_body = world_->GetBody(proj_body_ref);
  //proj_body.SetPosition(body.Position() + Math::Vec2F(0.5f, 0.f));

  //const auto& col_ref = world_->CreateCollider(proj_body_ref);
  //auto& collider = world_->GetCollider(col_ref);

  //collider.SetShape(Math::CircleF(Math::Vec2F::Zero(), 0.1f));
  //collider.SetRestitution(0.f);
}

void PlayerController::OnTriggerEnter(PhysicsEngine::ColliderRef colliderRefA,
    PhysicsEngine::ColliderRef colliderRefB) noexcept {

  if (colliderRefA == jump_col_ref_ || colliderRefB == jump_col_ref_) {
    if (colliderRefA == main_col_ref_ || colliderRefB == main_col_ref_) {
      return;
    }

    can_jump_ = true;
    is_jumping_ = false;
  }

  //if (colliderRefA == platforms_manager_.platform_col_refs[0] ||
  //    colliderRefB == platforms_manager_.platform_col_refs[0]) {
  //  if (colliderRefA == player_controller_.jump_col_ref_ ||
  //      colliderRefB == player_controller_.jump_col_ref_) {
  //    std::cout << "trigger enter \n";
  //    player_controller_.can_jump_ = true;
  //    player_controller_.is_jumping_ = false;
  //  }
  //}
}

Math::Vec2F PlayerController::GetPlayerPosition() const noexcept {
  const auto& body_ref = world_->GetCollider(main_col_ref_).GetBodyRef();
  const auto& body = world_->GetBody(body_ref);
  return body.Position();
}

Math::Vec2F PlayerController::GetJumpColliderPosition() const noexcept {
  const auto& body_ref = world_->GetCollider(main_col_ref_).GetBodyRef();
  const auto& body = world_->GetBody(body_ref);
  return body.Position() + game_constants::kPlayerJumpColOffset;
}

Math::CircleF PlayerController::GetJumpColliderShape() const noexcept {
  const auto& col = world_->GetCollider(jump_col_ref_);
  return std::get<Math::CircleF>(col.Shape());
}
