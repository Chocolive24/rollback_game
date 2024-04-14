#include "game_renderer.h"

#include "engine.h"
#include "Metrics.h"
#include "texture_manager.h"

using namespace raylib;

#include <iostream>

GameRenderer::GameRenderer(GameManager* game_manager) noexcept
    : game_manager_(game_manager) {}

void GameRenderer::Init() noexcept {
  texture_manager::CreateAllSprites();

  camera_.target = Vector2{0.f, 0.f};
  camera_.offset = Vector2{0.f, 0.f};
  camera_.zoom = 1.f;
  camera_.rotation = 0.f;
}

void GameRenderer::Draw() noexcept {
  // Calculate aspect ratios
  constexpr float game_aspect_ratio =
      static_cast<float>(game_constants::kGameScreenSize.X) /
      game_constants::kGameScreenSize.Y;

  const float windowAspectRatio = static_cast<float>(GetScreenWidth()) /
                                  static_cast<float> (GetScreenHeight());

  // Calculate scale and offsets
  float scale;
  Vector2 offset;
  if (game_aspect_ratio > windowAspectRatio) {
    // Game screen is wider than the window
    scale = static_cast<float>(GetScreenWidth()) /
            game_constants::kGameScreenSize.X;
    offset = {
        0,
        (GetScreenHeight() - game_constants::kGameScreenSize.Y * scale) * 0.5f};
  }
  else {
    // Game screen is taller than the window
    scale = static_cast<float>(GetScreenHeight()) /
            game_constants::kGameScreenSize.Y;
    offset = {
        (GetScreenWidth() - game_constants::kGameScreenSize.X * scale) * 0.5f,
        0};
  }

  // Adjust zoom level based on screen size
  const float minZoom = fminf(
      static_cast<float>(GetScreenWidth()) / game_constants::kGameScreenSize.X,
      static_cast<float>(GetScreenHeight()) /
          game_constants::kGameScreenSize.Y);
  camera_.zoom = minZoom;
  camera_.offset = offset;

  BeginMode2D(camera_);
  {
    ClearBackground(BLACK);

    DrawCircle(-200.f, 0.f, 50.f, RED);

    DrawPlatforms();
    DrawPlayer();
  }
  EndMode2D();
}

void GameRenderer::Deinit() noexcept {
  texture_manager::DestroyAllSprites();
}

void GameRenderer::DrawPlatforms() const noexcept {
  for (std::size_t i = 0; i < game_constants::kPlatformCount; i++) {
    const auto pos = game_manager_->platform_manager().GetPlatformPosition(i);
    const auto pixel_pos = Metrics::MetersToPixels(pos);

    const auto col_size =
        game_manager_->platform_manager().GetPlatformShape(i).Size();
    const auto col_pix_size = Metrics::MetersToPixels(col_size);

    const auto centered_pix_pos = pixel_pos - col_pix_size * 0.5f;

    DrawRectangle(centered_pix_pos.X, centered_pix_pos.Y, col_pix_size.X,
                  col_pix_size.Y, BLUE);

    // Draw collider if in debug mode.
    // ===============================
#ifdef DEBUG
    DrawRectangleLines(centered_pix_pos.X, centered_pix_pos.Y, col_pix_size.X,
                       col_pix_size.Y,
                       RED);
#endif
    // ===============================
  }
}

void GameRenderer::DrawPlayer() const noexcept {
  const auto player_pos =
      game_manager_->player_controller().GetPlayerPosition();
  const auto player_pix_pos = Metrics::MetersToPixels(player_pos);
  constexpr auto main_col_pix_length =
      Metrics::MetersToPixels(game_constants::kPlayerMainColLength);

  texture_manager::penguin.Draw(Vector2{player_pix_pos.X, player_pix_pos.Y});

   // Draw colliders if in debug mode.
   // ================================
#ifdef DEBUG
  DrawRectangleLines(player_pix_pos.X - main_col_pix_length * 0.5f,
                     player_pix_pos.Y - main_col_pix_length * 0.5f,
                     main_col_pix_length, main_col_pix_length, RED);

  const auto jump_col_pos =
      game_manager_->player_controller().GetJumpColliderPosition();
  const auto jump_col_pix_pos = Metrics::MetersToPixels(jump_col_pos);

  const auto radius =
      game_manager_->player_controller().GetJumpColliderShape().Radius();

  const auto pix_radius = Metrics::MetersToPixels(radius);

  DrawCircleLines(jump_col_pix_pos.X, jump_col_pix_pos.Y, pix_radius, RED);
#endif
  // ================================
}
