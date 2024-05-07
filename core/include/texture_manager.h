#pragma once

#include <string_view>

#include "raylib_wrapper.h"

enum class Pivot : char {
  kCenter,
  kTopLeft,
  kTopRight,
  kDownLeft,
  kDownRight,
  kDownCenter,
  kBackGround
};

class Sprite {
 public:
   raylib::Texture2D tex;
   raylib::Vector2 pos;
   raylib::Vector2 origin;
   raylib::Rectangle source;
   raylib::Rectangle dest;

  void Draw(raylib::Vector2 position, float rotation = 0,
             raylib::Color color = raylib::Color{255, 255, 255, 255});
};

Sprite CreateSprite(std::string_view path, raylib::Vector2 scale,
                    Pivot pivot = Pivot::kCenter);


struct PenguinAnimations {
  raylib::Texture2D idle_animation;
  /**
   * \brief Total frame count in the idle animation texture.
   */
  static constexpr std::int8_t kIdleAnimFrameCount = 2;
  static constexpr float kIdleAnimFrameRate = 3.f;
  raylib::Rectangle idle_anim_rec;
  float idle_anim_frame_counter;

  raylib::Texture2D spin_animation;
  /**
   * \brief Total frame count in the spin animation texture.
   */
  static constexpr std::int8_t kSpinAnimFrameCount = 7;
  static constexpr float kSpinAnimFrameRate = 16.f;
  raylib::Rectangle spin_anim_rec;
  float spin_anim_frame_counter;

  raylib::Texture2D walk_animation;
  /**
   * \brief Total frame count in the walk animation texture.
   */
  static constexpr std::int8_t kWalkAnimFrameCount = 6;
  static constexpr float kWalkAnimFrameRate = 14.f;
  raylib::Rectangle walk_anim_rec;
  float walk_anim_frame_counter;

  raylib::Texture2D hurt_animation;
  /**
   * \brief Total frame count in the hurt animation texture.
   */
  static constexpr std::int8_t kHurtAnimFrameCount = 4;
  static constexpr float kHurtAnimFrameRate = 12.f;
  raylib::Rectangle hurt_anim_rec;
  float hurt_anim_frame_counter;

  void Create(std::string_view directory_path) noexcept;
  void Destroy() noexcept;
};

class TextureManager {
public:
  void CreateAllSprites() noexcept;
  void DestroyAllSprites() noexcept;

  Sprite ice_ground{};
  Sprite log{};
  Sprite spikes{};
  Sprite snow_ball{};

  PenguinAnimations player1_animations{};
  PenguinAnimations player2_animations{};
};
