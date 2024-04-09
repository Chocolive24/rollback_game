#include "game_renderer.h"
#include "Metrics.h"
#include "texture_manager.h"

#include <raylib.h>

#include <iostream>

void GameRenderer::Init() noexcept {
  GameManager::Init();
}

void GameRenderer::Draw() noexcept {

  for (const auto& platform : platforms_manager_.platform_col_refs) {
    const auto& body_ref = world_.GetCollider(platform).GetBodyRef();
    const auto& body = world_.GetBody(body_ref);
    auto pos = body.Position();

    const auto& col = world_.GetCollider(platform);
    const auto rect = std::get<Math::RectangleF>(col.Shape());
    const auto col_size_pixels = Metrics::MetersToPixels(rect.Size());

    pos = Metrics::MetersToPixels(pos);
    const auto centered_pos = pos - col_size_pixels * 0.5f;

    DrawRectangle(centered_pos.X, centered_pos.Y, col_size_pixels.X,
                  col_size_pixels.Y, BLUE);

#ifdef DEBUG
    DrawRectangleLines(centered_pos.X, centered_pos.Y,
                       col_size_pixels.X, col_size_pixels.Y, RED);
#endif
  }

  for (const auto& player_controller : player_controllers_) {
    const auto& body_ref =
        world_.GetCollider(player_controller.col_ref_).GetBodyRef();
    const auto& body = world_.GetBody(body_ref);
    auto ball_pos = body.Position();
    ball_pos = Metrics::MetersToPixels(ball_pos);
    const auto radius =
        Metrics::MetersToPixels(game_constants::kPlayerColLength);


    //DrawCircle(ball_pos.X, ball_pos.Y,
    //           Metrics::MetersToPixels(game_constants::kPlayerColLength), RED);
    texture_manager::penguin.Draw({ball_pos.X, ball_pos.Y});

#ifdef DEBUG
    DrawRectangleLines(ball_pos.X - radius * 0.5f, ball_pos.Y - radius * 0.5f,
                       radius, radius, RED);
#endif
  }
}

void GameRenderer::Deinit() noexcept {
  GameManager::Deinit();
}