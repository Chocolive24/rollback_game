#include "player_controller.h"

#include <iostream>

#include "game_constants.h"
#include "inputs.h"

void PlayerController::Init() noexcept {
  const auto& body_ref = world_->CreateBody();
  auto& body = world_->GetBody(body_ref);
  body.SetPosition(game_constants::kPlayerStartPos);
  body.SetDamping(0.3f);
  col_ref_ = world_->CreateCollider(body_ref);
  auto& collider = world_->GetCollider(col_ref_);

  constexpr auto half_size = Math::Vec2F(game_constants::kPlayerColLength * 0.5f,
                                     game_constants::kPlayerColLength * 0.5f);
  collider.SetShape(Math::RectangleF(-half_size, half_size));
  collider.SetRestitution(0.1f);

  can_jump_col_ref_ = world_->CreateCollider(body_ref);
  auto& can_jump_col = world_->GetCollider(can_jump_col_ref_);

  can_jump_col.SetShape(Math::CircleF(Math::Vec2F::Zero(), 1.f));
  can_jump_col.SetIsTrigger(true);
}

void PlayerController::Update() const noexcept {
  if (move_direction_.Length() >= Math::Epsilon) {
    const auto& body_ref = world_->GetCollider(col_ref_).GetBodyRef();
    auto& body = world_->GetBody(body_ref);
    const auto dir = move_direction_.Length() >= Math::Epsilon
                         ? move_direction_.Normalized()
                         : move_direction_;
    const auto val = dir * game_constants::kPlayerSpeedMoveFactor;
    body.ApplyForce(val);
  }
}

void PlayerController::PollInputs() noexcept {
  const auto inputs = inputs::GetPlayerInputs(1);
  move_direction_ = Math::Vec2F::Zero();

  if (inputs & static_cast<inputs::PlayerInputs>(inputs::PlayerInputTypes::kUp)) {
    move_direction_ += Math::Vec2F::Down();
  }
  if (inputs & static_cast<inputs::PlayerInputs>(inputs::PlayerInputTypes::kLeft)) {
    move_direction_ += Math::Vec2F::Left();
  }
  if (inputs & static_cast<inputs::PlayerInputs>(inputs::PlayerInputTypes::kDown)) {
    move_direction_ += Math::Vec2F::Up();
  }
  if (inputs & static_cast<inputs::PlayerInputs>(inputs::PlayerInputTypes::kRight)) {
    move_direction_ += Math::Vec2F::Right();
  }

  if (inputs & static_cast<inputs::PlayerInputs>(inputs::PlayerInputTypes::kJump)) {
    if (!can_jump_) {
      return;
    }

    is_jumping_ = true;
    can_jump_ = false;
    const auto& body_ref = world_->GetCollider(col_ref_).GetBodyRef();
    auto& body = world_->GetBody(body_ref);

    body.ApplyForce(Math::Vec2F::Down() * game_constants::kPlayerJumpMagnitude);
  }
}
