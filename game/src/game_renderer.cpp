#include "game_renderer.h"
#include "Metrics.h"

#include <raylib.h>

#include <iostream>

void GameRenderer::Init() noexcept {
  GameManager::Init();
}

void GameRenderer::Draw() noexcept {
  static Texture2D tex = LoadTexture("data/images/Penguin.png");

  for (const auto& player_controller : player_controllers_) {
    const auto& body_ref =
        world_.GetCollider(player_controller.col_ref_).GetBodyRef();
    const auto& body = world_.GetBody(body_ref);
    auto ball_pos = body.Position();
    ball_pos = Metrics::MetersToPixels(ball_pos);

    

    DrawCircle(ball_pos.X, ball_pos.Y,
               Metrics::MetersToPixels(game_constants::kPlayerColRadius), RED);
    DrawTextureEx(tex, {ball_pos.X, ball_pos.Y}, 0.f, 4.f, WHITE);

  }
}

void GameRenderer::Deinit() noexcept {
  GameManager::Deinit();
}