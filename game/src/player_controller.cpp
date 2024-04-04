#include "player_controller.h"

#include <iostream>
#include <raylib.h>

#include "game_constants.h"

void PlayerController::Init() noexcept {
  const auto& body_ref = world_->CreateBody();
  auto& body = world_->GetBody(body_ref);
  body.SetPosition(game_constants::kPlayerStartPos);
  col_ref_ = world_->CreateCollider(body_ref);
  auto& collider = world_->GetCollider(col_ref_);
  collider.SetShape(Math::CircleF(body.Position(), game_constants::kPlayerColRadius));
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
  move_direction_ = Math::Vec2F::Zero();

  if (IsKeyDown(KEY_W)) {
    move_direction_ += Math::Vec2F::Down();
  }
  if (IsKeyDown(KEY_A)) {
    move_direction_ += Math::Vec2F::Left();
  }
  if (IsKeyDown(KEY_S)) {
    move_direction_ += Math::Vec2F::Up();
  }
  if (IsKeyDown(KEY_D)) {
    move_direction_ += Math::Vec2F::Right();
  }
}
