#include "game_renderer.h"

#include "Metrics.h"
#include "texture_manager.h"

using namespace raylib;

#include <iostream>

GameRenderer::GameRenderer(GameManager* game_manager) noexcept
    : game_manager_(game_manager) {}

void GameRenderer::Init() const noexcept {
  texture_manager::CreateAllSprites();
}

void GameRenderer::Draw() const noexcept {
  static RenderTexture2D target =
      LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

  BeginTextureMode(target); {
    ClearBackground(BLACK);

    DrawPlatforms();
    DrawPlayer();
  }
  EndTextureMode();

  // Draw the render texture scaled to fit the window
  DrawTexturePro(target.texture,
      Rectangle{0.0f, 0.0f, (float)target.texture.width,
                (float)-target.texture.height},
      Rectangle{0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()},
      Vector2{0, 0}, 0.0f, WHITE);
}

void GameRenderer::Deinit() const noexcept {
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

  texture_manager::penguin.Draw({player_pix_pos.X, player_pix_pos.Y});

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
