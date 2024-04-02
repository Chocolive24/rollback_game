#include "game_manager.h"

#include <iostream>
#include <raylib.h>

void GameManager::Init() noexcept {
  world_.Init(Math::Vec2F(0.f, 0.5f));

  timer_.Init();

  const auto& body_ref = world_.CreateBody();
  auto& body = world_.GetBody(body_ref);
  body.SetPosition(kStartBallPos);
  ball_col_ref_ = world_.CreateCollider(body_ref);
  auto& collider = world_.GetCollider(ball_col_ref_);
  collider.SetShape(Math::CircleF(body.Position(), kBallRadius));
}

void GameManager::PollInputs() noexcept {
  move_direction_ = Math::Vec2F::Zero();

  if (IsKeyDown(KEY_W)) {
    move_direction_ += Math::Vec2F::Up();
  }
  if (IsKeyDown(KEY_A)) {
    move_direction_ += Math::Vec2F::Left();
  }
  if (IsKeyDown(KEY_S)) {
    move_direction_ += Math::Vec2F::Down();
  }
  if (IsKeyDown(KEY_D)) {
    move_direction_ += Math::Vec2F::Right();
  }
}

void GameManager::Update() noexcept {
  timer_.Tick();

  //if (move_direction_.Length() >= Math::Epsilon) {
  //  const auto& body_ref = world_.GetCollider(ball_col_ref_).GetBodyRef();
  //  auto& body = world_.GetBody(body_ref);
  //  const auto val = move_direction_.Normalized() * kMaxSpeedAmplitude;
  //  //std::cout << val.X << " " << val.Y << '\n';
  //  body.SetPosition(body.Position() + val * timer_.DeltaTime());
  //}

  world_.Update(1/50.f);
}

void GameManager::Deinit() noexcept {
  world_.Deinit();
}
