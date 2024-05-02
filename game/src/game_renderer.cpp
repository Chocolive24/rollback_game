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

  auto& body = graphic_bodies_[0];
  body.SetPosition(game_constants::kPlayer1StartPos);
  body.SetDamping(PlayerManager::kPlayerDamping);
  body.SetMass(PlayerManager::kPlayerMass);
}

void GameRenderer::Update(float delta_time) noexcept {
  UpdateObjectsGraphicPositions(delta_time);
}

void GameRenderer::FixedUpdate() noexcept {
  //graphic_bodies_[0].ApplyForce(
  //    game_manager_->player_manager().GetPlayerForces(0));
}

void GameRenderer::Draw(const RenderTexture2D& render_target,
                        Vector2 render_target_pos, float time_since_last_fixed_update) noexcept {
  UpdateCamera(render_target, render_target_pos);

  BeginTextureMode(render_target); {
    BeginMode2D(camera_); {
      ClearBackground(BLACK);

      for (int y = 0; y < 720; y+=36) {
        for (int x = 0; x < 1344; x+=64) {
          texture_manager::ice_ground.Draw(Vector2{static_cast<float>(x), 
            static_cast<float>(y)});
        }
      }

      for (int x = 0; x < 1344; x+=188)
      {
        texture_manager::fire.Draw(
            Vector2{static_cast<float>(x), static_cast<float>(24)});

        texture_manager::fire.Draw(
            Vector2{static_cast<float>(x), static_cast<float>(720 - 24)}, 180.f);
      }

      for (int y = 94; y < 720; y += 188) {
        texture_manager::fire.Draw(
            Vector2{static_cast<float>(24), static_cast<float>(y)}, -90);

        texture_manager::fire.Draw(
            Vector2{static_cast<float>(1280 - 24), static_cast<float>(y)}, 90);
      }

      DrawPlatforms();
      DrawProjectiles();
      DrawPlayer(time_since_last_fixed_update);
    }
    EndMode2D();

    game_gui_.Draw(render_target);

    if (game_manager_->is_finished()) {
      const auto local_player_hp = 
          game_manager_->player_manager().GetPlayerHp(game_manager_->player_id());
      const std::string end_txt = local_player_hp <= 0 ? "You lost !" : "You won !";

      constexpr int txt_size = 60;

      const auto render_target_center_x = render_target.texture.width / 2 -
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

void GameRenderer::UpdateObjectsGraphicPositions(float delta_time) noexcept {
  return;
  graphic_bodies_[0].ApplyForce(
      game_manager_->player_manager().GetPlayerVelocity(0));

  for (auto& body : graphic_bodies_)
  {
    if (!body.IsValid()) continue;

    switch (body.GetBodyType()) {
    case PhysicsEngine::BodyType::Dynamic: {

      // a = F / m
      Math::Vec2F acceleration = body.Forces() * body.InverseMass();

      // Change velocity according to delta time.
      body.SetVelocity(body.Velocity() + acceleration * delta_time);

      // Apply damping to velocity according to delta time.
      body.SetVelocity(body.Velocity() * (1.0f - body.Damping() * delta_time));

      // Change position according to velocity and delta time.
      body.SetPosition(body.Position() + body.Velocity() * delta_time);

      body.ResetForces();

      break;
    }

    case PhysicsEngine::BodyType::Kinematic: {
      // Kinematic bodies are not impacted by forces.

      // Change position according to velocity and delta time.
      body.SetPosition(body.Position() + body.Velocity() * delta_time);

      break;
    }

    case PhysicsEngine::BodyType::Static:
    case PhysicsEngine::BodyType::None:
      break;
    }
  }
}


void GameRenderer::UpdateCamera(const RenderTexture2D& render_target, Vector2 render_target_pos) {
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
                  0.5f,0};
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

void GameRenderer::DrawPlatforms() const noexcept {
  for (std::size_t i = 0; i < game_constants::kArenaBorderWallCount; i++) {
    const auto pos = ArenaManager::border_wall_positions[i];
    const auto pixel_pos = Metrics::MetersToPixels(pos);

    const auto col_size = ArenaManager::wall_shapes[i].Size();
    const auto col_pix_size = Metrics::MetersToPixels(col_size);

    const auto centered_pix_pos = pixel_pos - col_pix_size * 0.5f;

    //DrawRectangle(centered_pix_pos.X, centered_pix_pos.Y, col_pix_size.X,
    //              col_pix_size.Y, PURPLE);

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

    
     //DrawRectangle(centered_pix_pos.X, centered_pix_pos.Y, col_pix_size.X,
     //              col_pix_size.Y, GRAY);

    texture_manager::log.Draw({pixel_pos.X - 18.f, pixel_pos.Y});
    texture_manager::log.Draw({pixel_pos.X + 23.f, pixel_pos.Y});

  }
}

void GameRenderer::DrawProjectiles() const noexcept {
  for (std::size_t i = 0; i < ProjectileManager::kMaxProjectileCount; i++) {

    if (!game_manager_->projectile_manager().IsProjectileEnabled(i)) {
      // Projectile not enabled.
      continue;
    }

    const auto proj_pos =
        game_manager_->projectile_manager().GetProjectilePosition(i);
    const auto proj_pix_pos = Metrics::MetersToPixels(proj_pos);
    const auto proj_pix_pos_int = static_cast<Math::Vec2I>(proj_pix_pos);

    const auto radius =
        game_manager_->projectile_manager().GetProjectileCircle(i).Radius();
    const auto pix_radius = Metrics::MetersToPixels(radius);


    DrawCircle(proj_pix_pos_int.X, proj_pix_pos_int.Y, pix_radius, WHITE);
  }
}

void GameRenderer::DrawPlayer(float time_since_last_fixed_update) const noexcept {
  for (std::size_t i = 0; i < game_constants::kMaxPlayerCount; i++) {
    auto player_pos =
      game_manager_->player_manager().GetPlayerPosition(i);

    // Calculate the position that the player must have based on the time since the
    // last fixed update.
    player_pos += game_manager_->player_manager().GetPlayerVelocity(i) *
                    time_since_last_fixed_update;

    const auto player_pix_pos = Metrics::MetersToPixels(player_pos);

    if (i == 0)
    {
      texture_manager::penguin_blue.Draw(
          Vector2{player_pix_pos.X, player_pix_pos.Y});
    }
    else
    {
      texture_manager::penguin_red.Draw(
          Vector2{player_pix_pos.X, player_pix_pos.Y});
    }
    
    // Draw colliders if in debug mode.
    // ================================
#ifdef DEBUG
    constexpr auto main_col_pix_length =
        Metrics::MetersToPixels(game_constants::kPlayerMainColLength);
    DrawRectangleLines(player_pix_pos.X - main_col_pix_length * 0.5f,
                       player_pix_pos.Y - main_col_pix_length * 0.5f,
                       main_col_pix_length, main_col_pix_length, RED);
    
    //const auto jump_col_pos =
    //    game_manager_->player_manager().GetJumpColliderPosition(i);
    //const auto jump_col_pix_pos = Metrics::MetersToPixels(jump_col_pos);
    //
    //const auto radius =
    //    game_manager_->player_manager().GetJumpColliderShape(i).Radius();
    //
    //const auto pix_radius = Metrics::MetersToPixels(radius);
    //
    //DrawCircleLines(jump_col_pix_pos.X, jump_col_pix_pos.Y, pix_radius, RED);
#endif
    // ================================
  }
}
