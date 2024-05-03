#include "game_renderer.h"

#include "Metrics.h"
#include "engine.h"
#include "texture_manager.h"

using namespace raylib;

#include <iostream>

GameRenderer::GameRenderer(LocalGameManager* game_manager) noexcept
    : game_manager_(game_manager) {
  game_gui_.Init(game_manager);
}

void GameRenderer::Init() noexcept {
  texture_manager::CreateAllSprites();

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

      for (int y = 0; y < 720; y += 36) {
        for (int x = 0; x < 1344; x += 64) {
          texture_manager::ice_ground.Draw(
              Vector2{static_cast<float>(x), static_cast<float>(y)});
        }
      }

      for (int x = 0; x < 1344; x += 188) {
        texture_manager::spikes.Draw(
            Vector2{static_cast<float>(x), static_cast<float>(24)});

        texture_manager::spikes.Draw(
            Vector2{static_cast<float>(x), static_cast<float>(720 - 24)},
            180.f);
      }

      for (int y = 94; y < 720; y += 188) {
        texture_manager::spikes.Draw(
            Vector2{static_cast<float>(24), static_cast<float>(y)}, -90);

        texture_manager::spikes.Draw(
            Vector2{static_cast<float>(1280 - 24), static_cast<float>(y)}, 90);
      }

      DrawWalls();
      DrawProjectiles(time_since_last_fixed_update);
      DrawPlayers(time_since_last_fixed_update);
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

      constexpr int txt_size = 60;

      const auto render_target_center_x =
          render_target.texture.width / 2 -
          MeasureText(end_txt.c_str(), txt_size) / 2;
      const auto render_target_center_y =
          render_target.texture.height / 2 - txt_size / 2;

      raylib::DrawRaylibText(end_txt.c_str(), render_target_center_x,
                             render_target_center_y, txt_size, raylib::GRAY);
    }
  }
  EndTextureMode();
}

void GameRenderer::Deinit() noexcept { texture_manager::DestroyAllSprites(); }

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

void GameRenderer::DrawWalls() const noexcept {
  for (std::size_t i = 0; i < game_constants::kArenaBorderWallCount; i++) {
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

    texture_manager::log.Draw({pixel_pos.X - 18.f, pixel_pos.Y});
    texture_manager::log.Draw({pixel_pos.X + 23.f, pixel_pos.Y});
  }
}

void GameRenderer::DrawProjectiles(
    float time_since_last_fixed_update) const noexcept {
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

    DrawCircle(proj_pix_pos_int.X, proj_pix_pos_int.Y, pix_radius, WHITE);
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
    const auto& idle_anim = is_player_1 ? texture_manager::blue_idle_animation
                                        : texture_manager::red_idle_animation;

    const auto& spin_anim = is_player_1 ? texture_manager::blue_spin_animation
                                        : texture_manager::red_spin_animation;

    const auto& walk_anim = is_player_1 ? texture_manager::blue_walk_animation
                                        : texture_manager::red_walk_animation;

    const auto& hurt_anim = is_player_1 ? texture_manager::blue_hurt_animation
                                        : texture_manager::red_hurt_animation;

    const auto is_player_facing_right = 
        game_manager_->player_manager().IsPlayerFacingRight(i);

    if (game_manager_->player_manager().IsPlayerHurt(i)) {
      // Update animation frame counter
      texture_manager::hurt_anim_frame_counter +=
          raylib::GetFrameTime() * texture_manager::kHurtAnimFrameRate;

      // Check if it's time to advance to the next frame
      if (texture_manager::hurt_anim_frame_counter >
          texture_manager::kHurtAnimFrameCount) {
        texture_manager::hurt_anim_frame_counter -=
            texture_manager::kHurtAnimFrameCount;
      }

      // Calculate the current frame based on the animation frame counter
      const int current_frame =
          static_cast<int>(texture_manager::hurt_anim_frame_counter);

      // Update the x position of the source rectangle for the current frame
      texture_manager::hurt_anim_rec.x = static_cast<float>(current_frame) *
                                         static_cast<float>(hurt_anim.width) /
                                         texture_manager::kHurtAnimFrameCount;

      const auto width = is_player_facing_right
                             ? -texture_manager::hurt_anim_rec.width
                             : texture_manager::hurt_anim_rec.width;

      const raylib::Rectangle source = {texture_manager::hurt_anim_rec.x,
                                        texture_manager::hurt_anim_rec.y,
                                        width,
                                        texture_manager::hurt_anim_rec.height};

      // Draw the current frame of the animation
      DrawTextureRec(
          hurt_anim, source,
          Vector2{
              player_pix_pos.X - texture_manager::hurt_anim_rec.width / 2,
              player_pix_pos.Y - texture_manager::hurt_anim_rec.height / 2.5f},
          WHITE);

    }
    else if (game_manager_->player_manager().IsPlayerSpinning(i)) {
      // Update animation frame counter
      texture_manager::spin_anim_frame_counter +=
          raylib::GetFrameTime() * texture_manager::kSpinAnimFrameRate;

      // Check if it's time to advance to the next frame
      if (texture_manager::spin_anim_frame_counter >
          texture_manager::kSpinAnimFrameCount) {
        texture_manager::spin_anim_frame_counter -=
            texture_manager::kSpinAnimFrameCount;
      }

      // Calculate the current frame based on the animation frame counter
      const int current_frame =
          static_cast<int>(texture_manager::spin_anim_frame_counter);

      // Update the x position of the source rectangle for the current frame
      texture_manager::spin_anim_rec.x = static_cast<float>(current_frame) *
                                         static_cast<float>(spin_anim.width) /
                                         texture_manager::kSpinAnimFrameCount;

      const auto width = is_player_facing_right
                             ? -texture_manager::spin_anim_rec.width
                             : texture_manager::spin_anim_rec.width;

      const raylib::Rectangle source = {texture_manager::spin_anim_rec.x,
                                        texture_manager::spin_anim_rec.y,
                                        width,
                                        texture_manager::spin_anim_rec.height};

      // Draw the current frame of the animation
      DrawTextureRec(
          spin_anim, source,
          Vector2{
              player_pix_pos.X - texture_manager::spin_anim_rec.width / 2,
              player_pix_pos.Y - texture_manager::spin_anim_rec.height / 2.5f},
          WHITE);
    }

    else if (game_manager_->player_manager().IsPlayerWalking(i)) {
      // Update animation frame counter
      texture_manager::walk_anim_frame_counter +=
          raylib::GetFrameTime() * texture_manager::kWalkAnimFrameRate;

      // Check if it's time to advance to the next frame
      if (texture_manager::walk_anim_frame_counter >
          texture_manager::kWalkAnimFrameCount) {
        texture_manager::walk_anim_frame_counter -=
            texture_manager::kWalkAnimFrameCount;
      }

      // Calculate the current frame based on the animation frame counter
      const int current_frame =
          static_cast<int>(texture_manager::walk_anim_frame_counter);

      // Update the x position of the source rectangle for the current frame
      texture_manager::walk_anim_rec.x = static_cast<float>(current_frame) *
                                         static_cast<float>(walk_anim.width) /
                                         texture_manager::kWalkAnimFrameCount;

      const auto width = is_player_facing_right
                             ? -texture_manager::walk_anim_rec.width
                             : texture_manager::walk_anim_rec.width;

      const raylib::Rectangle source = {texture_manager::walk_anim_rec.x,
                                        texture_manager::walk_anim_rec.y,
                                        width,
                                        texture_manager::walk_anim_rec.height};

      // Draw the current frame of the animation
      DrawTextureRec(
          walk_anim, source,
          Vector2{
              player_pix_pos.X - texture_manager::walk_anim_rec.width / 2,
              player_pix_pos.Y - texture_manager::walk_anim_rec.height / 2.5f},
          WHITE);
    }

    else {
      // Update animation frame counter
      texture_manager::idle_anim_frame_counter +=
          raylib::GetFrameTime() * texture_manager::kIdleAnimFrameRate;

      // Check if it's time to advance to the next frame
      if (texture_manager::idle_anim_frame_counter >
          texture_manager::kIdleAnimFrameCount) {
        texture_manager::idle_anim_frame_counter -=
            texture_manager::kIdleAnimFrameCount;
      }

      // Calculate the current frame based on the animation frame counter
      const int current_frame =
          static_cast<int>(texture_manager::idle_anim_frame_counter);

      // Update the x position of the source rectangle for the current frame
      texture_manager::idle_anim_rec.x = static_cast<float>(current_frame) *
                                         static_cast<float>(idle_anim.width) /
                                         texture_manager::kIdleAnimFrameCount;

      const auto width = is_player_facing_right 
              ? -texture_manager::idle_anim_rec.width
              : texture_manager::idle_anim_rec.width;

      const raylib::Rectangle source = {texture_manager::idle_anim_rec.x,
                                        texture_manager::idle_anim_rec.y,
                                        width,
                                  texture_manager::idle_anim_rec.height};

       // Draw the current frame of the animation
      DrawTextureRec(
          idle_anim, source,
          Vector2{
              player_pix_pos.X - texture_manager::idle_anim_rec.width / 2,
              player_pix_pos.Y - texture_manager::idle_anim_rec.height / 2.5f},
          WHITE);
    }

    // Draw colliders if in debug mode.
    // ================================
#ifdef DEBUG
    constexpr auto main_col_pix_length =
        Metrics::MetersToPixels(game_constants::kPlayerMainColLength);
    DrawRectangleLines(player_pix_pos.X - main_col_pix_length * 0.5f,
                       player_pix_pos.Y - main_col_pix_length * 0.5f,
                       main_col_pix_length, main_col_pix_length, RED);

    // const auto jump_col_pos =
    //     game_manager_->player_manager().GetJumpColliderPosition(i);
    // const auto jump_col_pix_pos = Metrics::MetersToPixels(jump_col_pos);
    //
    // const auto radius =
    //     game_manager_->player_manager().GetJumpColliderShape(i).Radius();
    //
    // const auto pix_radius = Metrics::MetersToPixels(radius);
    //
    // DrawCircleLines(jump_col_pix_pos.X, jump_col_pix_pos.Y, pix_radius, RED);
#endif
    // ================================
  }
}
