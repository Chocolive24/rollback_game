#include "game_renderer.h"
#include "Metrics.h"

#include <iostream>

using namespace raylib;

GameRenderer::GameRenderer(LocalGameManager* game_manager) noexcept
    : game_manager_(game_manager) {
  game_gui_.Init(game_manager);
}

void GameRenderer::Init() noexcept {
  texture_manager_.CreateAllSprites();

  camera_.target = Vector2{0.f, 0.f};
  camera_.offset = Vector2{0.f, 0.f};
  camera_.zoom = 1.f;
  camera_.rotation = 0.f;
}

void GameRenderer::Draw(const RenderTexture2D& render_target,
                        Vector2 render_target_pos,
                        float time_since_last_fixed_update) noexcept {
  UpdateCamera(render_target, render_target_pos);

  BeginTextureMode(render_target);
  {
    BeginMode2D(camera_);
    {
      ClearBackground(BLACK);

      DrawWalls();
      DrawProjectiles(time_since_last_fixed_update);
      DrawPlayers(time_since_last_fixed_update);

      constexpr int width = 5000;
      constexpr int height = 5000;
      DrawRectangle(-width, 0, width, height, BLACK); // Left
      DrawRectangle(1280, 0, width, height, BLACK); // Right
      DrawRectangle(0, -height, width, height, BLACK); // Up
      DrawRectangle(0, 720, width, height, BLACK); // Down

    }
    EndMode2D();

    game_gui_.Draw(render_target);

#ifdef USE_DEBUG
    raylib::DrawFPS(80, 80);
#endif

    if (game_manager_->is_finished()) {
      const auto local_player_hp = game_manager_->player_manager().GetPlayerHp(
          game_manager_->player_id());
      const std::string end_txt =
          local_player_hp <= 0 ? "You lost !" : "You won !";

      constexpr int txt_size = 100;

      const auto render_target_center_x =
          render_target.texture.width / 2 -
          MeasureText(end_txt.c_str(), txt_size) / 2;
      const auto render_target_center_y =
          render_target.texture.height / 4 - txt_size / 2;

      raylib::DrawRaylibText(end_txt.c_str(), render_target_center_x,
                             render_target_center_y, txt_size, raylib::GRAY);
    }
  }
  EndTextureMode();
}

void GameRenderer::Deinit() noexcept { texture_manager_.DestroyAllSprites(); }

void GameRenderer::UpdateCamera(const RenderTexture2D& render_target,
                                Vector2 render_target_pos) {
  // Calculate aspect ratios
  constexpr float game_aspect_ratio =
      static_cast<float>(game_constants::kGameScreenSize.X) /
      game_constants::kGameScreenSize.Y;

  const float windowAspectRatio =
      static_cast<float>(render_target.texture.width) /
      static_cast<float>(render_target.texture.height);

  // Calculate scale and offsets
  float scale;
  Vector2 offset;
  if (game_aspect_ratio > windowAspectRatio) {
    // Game screen is wider than the window
    scale = static_cast<float>(render_target.texture.width) /
            game_constants::kGameScreenSize.X;
    offset = {0, (render_target.texture.height -
                  game_constants::kGameScreenSize.Y * scale) *
                     0.5f};
  } else {
    // Game screen is taller than the window
    scale = static_cast<float>(render_target.texture.height) /
            game_constants::kGameScreenSize.Y;
    offset = {(render_target.texture.width -
               game_constants::kGameScreenSize.X * scale) *
                  0.5f,
              0};
  }

  // Adjust zoom level based on screen size
  const float minZoom = fminf(static_cast<float>(render_target.texture.width) /
                                  game_constants::kGameScreenSize.X,
                              static_cast<float>(render_target.texture.height) /
                                  game_constants::kGameScreenSize.Y);
  camera_.zoom = minZoom;
  camera_.offset = offset;

  Vector2 mouse_position = GetMousePosition();

  // Adjust mouse position based on player's portion of the screen.
  // ==============================================================
  mouse_position.x -= render_target_pos.x;
  mouse_position.y -= render_target_pos.y;

  mouse_position.x -= offset.x;
  mouse_position.y -= offset.y;

  mouse_position.x /= scale;
  mouse_position.y /= scale;

  input::mouse_pos[game_manager_->player_id()] =
      Metrics::PixelsToMeters(Math::Vec2F(mouse_position.x, mouse_position.y));
}

void GameRenderer::DrawWalls() noexcept {
  for (int y = 0; y < 720; y += 36) {
    for (int x = 0; x < 1344; x += 64) {
      texture_manager_.ice_ground.Draw(
          Vector2{static_cast<float>(x), static_cast<float>(y)});
    }
  }

  for (int x = 0; x < 1344; x += 188) {
    texture_manager_.spikes.Draw(
        Vector2{static_cast<float>(x), static_cast<float>(24)});

    texture_manager_.spikes.Draw(
        Vector2{static_cast<float>(x), static_cast<float>(720 - 24)}, 180.f);
  }

  for (int y = 94; y < 720; y += 188) {
    texture_manager_.spikes.Draw(
        Vector2{static_cast<float>(24), static_cast<float>(y)}, -90);

    texture_manager_.spikes.Draw(
        Vector2{static_cast<float>(1280 - 24), static_cast<float>(y)}, 90);
  }

  for (std::size_t i = 0; i < game_constants::kArenaSquareWallCount; i++) {
    const auto pos = ArenaManager::border_wall_positions[i];
    const auto pixel_pos = Metrics::MetersToPixels(pos);

    const auto col_size = ArenaManager::wall_shapes[i].Size();
    const auto col_pix_size = Metrics::MetersToPixels(col_size);

    const auto centered_pix_pos = pixel_pos - col_pix_size * 0.5f;

    // Draw collider if in debug mode.
    // ===============================
#ifdef DEBUG
    DrawRectangleLines(centered_pix_pos.X, centered_pix_pos.Y, col_pix_size.X,
                       col_pix_size.Y, RED);
#endif
    // ===============================
  }

  for (std::size_t i = 0; i < game_constants::kArenaSquareWallCount; i++) {
    const auto pos = ArenaManager::square_wall_positions[i];
    const auto pixel_pos = Metrics::MetersToPixels(pos);

    constexpr auto col_size = game_constants::kSquareWallRect.Size();
    constexpr auto col_pix_size = Metrics::MetersToPixels(col_size);

    const auto centered_pix_pos = pixel_pos - col_pix_size * 0.5f;

    texture_manager_.log.Draw({pixel_pos.X - 18.f, pixel_pos.Y});
    texture_manager_.log.Draw({pixel_pos.X + 23.f, pixel_pos.Y});
  }
}

void GameRenderer::DrawProjectiles(
    float time_since_last_fixed_update) noexcept {
  for (std::size_t i = 0; i < ProjectileManager::kMaxProjectileCount; i++) {
    if (!game_manager_->projectile_manager().IsProjectileEnabled(i)) {
      // Projectile not enabled.
      continue;
    }

    auto proj_pos =
        game_manager_->projectile_manager().GetProjectilePosition(i);

    const auto proj_vel =
        game_manager_->projectile_manager().GetProjectileVelocity(i);

    proj_pos += proj_vel * time_since_last_fixed_update;

    const auto proj_pix_pos = Metrics::MetersToPixels(proj_pos);
    const auto proj_pix_pos_int = static_cast<Math::Vec2I>(proj_pix_pos);

    const auto radius =
        game_manager_->projectile_manager().GetProjectileCircle(i).Radius();
    const auto pix_radius = Metrics::MetersToPixels(radius);

    texture_manager_.snow_ball.Draw(Vector2{proj_pix_pos.X, proj_pix_pos.Y});

#ifdef USE_DEBUG
    DrawCircleLines(proj_pix_pos_int.X, proj_pix_pos_int.Y, pix_radius, RED);
#endif
  }
}

void GameRenderer::DrawPlayers(float time_since_last_fixed_update) noexcept {
  for (std::size_t i = 0; i < game_constants::kMaxPlayerCount; i++) {
    auto player_pos = game_manager_->player_manager().GetPlayerPosition(i);

    // Calculate the position that the player must have based on the time since
    // the last fixed update.
    player_pos += game_manager_->player_manager().GetPlayerVelocity(i) *
                  time_since_last_fixed_update;

    const auto player_pix_pos = Metrics::MetersToPixels(player_pos);

    const auto is_player_1 = i == 0;

    auto& player_anim = is_player_1 ? texture_manager_.player1_animations
                                    : texture_manager_.player2_animations;

    const auto is_player_facing_right = 
        game_manager_->player_manager().IsPlayerFacingRight(i);

    if (game_manager_->player_manager().IsPlayerHurt(i)) {
      // Update animation frame counter
      player_anim.hurt_anim_frame_counter +=
          raylib::GetFrameTime() * player_anim.kHurtAnimFrameRate;

      // Check if it's time to advance to the next frame
      if (player_anim.hurt_anim_frame_counter >
          player_anim.kHurtAnimFrameCount) {
        player_anim.hurt_anim_frame_counter -=
            player_anim.kHurtAnimFrameCount;
      }

      // Calculate the current frame based on the animation frame counter
      const int current_frame =
          static_cast<int>(player_anim.hurt_anim_frame_counter);

      // Update the x position of the source rectangle for the current frame
      player_anim.hurt_anim_rec.x = static_cast<float>(current_frame) *
                                         static_cast<float>(player_anim.hurt_animation.width) /
                                         player_anim.kHurtAnimFrameCount;

      const auto width = is_player_facing_right
                             ? -player_anim.hurt_anim_rec.width
                             : player_anim.hurt_anim_rec.width;

      const raylib::Rectangle source = {player_anim.hurt_anim_rec.x,
                                        player_anim.hurt_anim_rec.y,
                                        width,
                                        player_anim.hurt_anim_rec.height};

      // Draw the current frame of the animation
      DrawTextureRec(
          player_anim.hurt_animation, source,
          Vector2{
              player_pix_pos.X - player_anim.hurt_anim_rec.width / 2,
              player_pix_pos.Y - player_anim.hurt_anim_rec.height / 2.5f},
          WHITE);

    }
    else if (game_manager_->player_manager().IsPlayerSpinning(i)) {
      // Update animation frame counter
      player_anim.spin_anim_frame_counter +=
          raylib::GetFrameTime() * player_anim.kSpinAnimFrameRate;

      // Check if it's time to advance to the next frame
      if (player_anim.spin_anim_frame_counter >
          player_anim.kSpinAnimFrameCount) {
        player_anim.spin_anim_frame_counter -=
            player_anim.kSpinAnimFrameCount;
      }

      // Calculate the current frame based on the animation frame counter
      const int current_frame =
          static_cast<int>(player_anim.spin_anim_frame_counter);

      // Update the x position of the source rectangle for the current frame
      player_anim.spin_anim_rec.x = static_cast<float>(current_frame) *
                                         static_cast<float>(player_anim.spin_animation.width) /
                                         player_anim.kSpinAnimFrameCount;

      const auto width = is_player_facing_right
                             ? -player_anim.spin_anim_rec.width
                             : player_anim.spin_anim_rec.width;

      const raylib::Rectangle source = {player_anim.spin_anim_rec.x,
                                        player_anim.spin_anim_rec.y,
                                        width,
                                        player_anim.spin_anim_rec.height};

      // Draw the current frame of the animation
      DrawTextureRec(
          player_anim.spin_animation, source,
          Vector2{
              player_pix_pos.X - player_anim.spin_anim_rec.width / 2,
              player_pix_pos.Y - player_anim.spin_anim_rec.height / 2.5f},
          WHITE);
    }

    else if (game_manager_->player_manager().IsPlayerWalking(i)) {
      // Update animation frame counter
      player_anim.walk_anim_frame_counter +=
          raylib::GetFrameTime() * player_anim.kWalkAnimFrameRate;

      // Check if it's time to advance to the next frame
      if (player_anim.walk_anim_frame_counter >
          player_anim.kWalkAnimFrameCount) {
        player_anim.walk_anim_frame_counter -=
            player_anim.kWalkAnimFrameCount;
      }

      // Calculate the current frame based on the animation frame counter
      const int current_frame =
          static_cast<int>(player_anim.walk_anim_frame_counter);

      // Update the x position of the source rectangle for the current frame
      player_anim.walk_anim_rec.x = static_cast<float>(current_frame) *
                                         static_cast<float>(player_anim.walk_animation.width) /
                                         player_anim.kWalkAnimFrameCount;

      const auto width = is_player_facing_right
                             ? -player_anim.walk_anim_rec.width
                             : player_anim.walk_anim_rec.width;

      const raylib::Rectangle source = {player_anim.walk_anim_rec.x,
                                        player_anim.walk_anim_rec.y,
                                        width,
                                        player_anim.walk_anim_rec.height};

      // Draw the current frame of the animation
      DrawTextureRec(
          player_anim.walk_animation, source,
          Vector2{
              player_pix_pos.X - player_anim.walk_anim_rec.width / 2,
              player_pix_pos.Y - player_anim.walk_anim_rec.height / 2.5f},
          WHITE);
    }

    else {
      // Update animation frame counter
      player_anim.idle_anim_frame_counter +=
          raylib::GetFrameTime() * player_anim.kIdleAnimFrameRate;

      // Check if it's time to advance to the next frame
      if (player_anim.idle_anim_frame_counter >
          player_anim.kIdleAnimFrameCount) {
        player_anim.idle_anim_frame_counter -=
            player_anim.kIdleAnimFrameCount;
      }

      // Calculate the current frame based on the animation frame counter
      const int current_frame =
          static_cast<int>(player_anim.idle_anim_frame_counter);

      // Update the x position of the source rectangle for the current frame
      player_anim.idle_anim_rec.x = static_cast<float>(current_frame) *
                                         static_cast<float>(player_anim.idle_animation.width) /
                                         player_anim.kIdleAnimFrameCount;

      const auto width = is_player_facing_right 
              ? -player_anim.idle_anim_rec.width
              : player_anim.idle_anim_rec.width;

      const raylib::Rectangle source = {player_anim.idle_anim_rec.x,
                                        player_anim.idle_anim_rec.y,
                                        width,
                                  player_anim.idle_anim_rec.height};

       // Draw the current frame of the animation
      DrawTextureRec(
          player_anim.idle_animation, source,
          Vector2{
              player_pix_pos.X - player_anim.idle_anim_rec.width / 2,
              player_pix_pos.Y - player_anim.idle_anim_rec.height / 2.5f},
          WHITE);
    }

    // Draw colliders if in debug mode.
    // ================================
#ifdef DEBUG
    constexpr auto main_col_pix_length =
        Metrics::MetersToPixels(game_constants::kPlayerMainColLength);

    DrawCircleLines(player_pix_pos.X, player_pix_pos.Y, main_col_pix_length * 0.5f, RED);
#endif
    // ================================
  }
}
