#pragma once

#include "game_gui.h"
#include "local_game_manager.h"
#include "raylib_wrapper.h"

/**
 * \brief GameRenderer is a class which draws the game into a given render target.
 */
class GameRenderer {
public:
  explicit GameRenderer(LocalGameManager* game_manager) noexcept;

  void Init() noexcept;
  void Update(float delta_time) noexcept;
  void FixedUpdate() noexcept;
  void Draw(const raylib::RenderTexture2D& render_target, 
	  raylib::Vector2 render_target_pos) noexcept;
  void Deinit() noexcept;

private:
  void UpdateObjectsGraphicPositions(float delta_time) noexcept;
  void UpdateCamera(const raylib::RenderTexture2D& render_target, 
	  raylib::Vector2 render_target_pos);
  void DrawPlatforms() const noexcept;
  void DrawProjectiles() const noexcept;
  void DrawPlayer() const noexcept;

  LocalGameManager* game_manager_ = nullptr;
  GameGui game_gui_{};
  raylib::Camera2D camera_{};

  std::array<PhysicsEngine::Body, game_constants::kGameBodyCount>
      graphic_bodies_{};
};