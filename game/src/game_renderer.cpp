#include "game_renderer.h"
#include "Metrics.h"
#include "texture_manager.h"

#include <raylib.h>

#include <iostream>

#include "engine.h"

void GameRenderer::Init() noexcept {
  GameManager::Init();
  const auto window_size = Engine::window_size();
  camera_.target = Vector2{0.f, 0.f};

  camera_.offset = Vector2{0.f, 0.f};
  camera_.rotation = 0.0f;
  camera_.zoom = 1.0f;
}

void GameRenderer::Draw() noexcept {

  static RenderTexture2D target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

  BeginTextureMode(target);
  {
    ClearBackground(BLACK);
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

  //for (const auto& player_controller : player_controllers_) {
    const auto& body_ref =
        world_.GetCollider(player_controller_.col_ref_).GetBodyRef();
    const auto& body = world_.GetBody(body_ref);
    auto ball_pos = body.Position();
    ball_pos = Metrics::MetersToPixels(ball_pos);
    const auto radius = Metrics::MetersToPixels(game_constants::kPlayerColLength);

    texture_manager::penguin.Draw({ball_pos.X, ball_pos.Y});

#ifdef DEBUG
    DrawRectangleLines(ball_pos.X - radius * 0.5f, ball_pos.Y - radius * 0.5f,
                       radius, radius, RED);

    ball_pos += Math::Vec2F(0.f, radius * 0.5f);
    const auto& col = world_.GetCollider(player_controller_.can_jump_col_ref_);

    const auto circleA = std::get<Math::CircleF>(col.Shape()) + col.Offset() + body.Position();
    auto center = Metrics::MetersToPixels(circleA.Center());
    auto r = Metrics::MetersToPixels(circleA.Radius());

    DrawCircleLines(center.X, center.Y, r, RED);
#endif

 // }
  }
  EndTextureMode();

  // Calculate scaling factors for the render texture
  float scaleX = (float)GetScreenWidth() / (float)target.texture.width;
  float scaleY = (float)GetScreenHeight() / (float)target.texture.height;

  // Draw the render texture scaled to fit the window
  DrawTexturePro(target.texture,
                 Rectangle{0.0f, 0.0f, (float)target.texture.width,
                             (float)-target.texture.height},
                 Rectangle{0.0f, 0.0f, (float)GetScreenWidth(),
                             (float)GetScreenHeight()},
                 Vector2{0, 0}, 0.0f, WHITE);

}

void GameRenderer::Deinit() noexcept {
  GameManager::Deinit();
}