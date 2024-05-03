#pragma once

#include <array>

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

namespace texture_manager {

void CreateAllSprites() noexcept;
void DestroyAllSprites() noexcept;

extern Sprite penguin_blue;
extern Sprite penguin_red;
extern Sprite ice_ground;
extern Sprite log;
extern Sprite spikes;

extern raylib::Texture2D blue_idle_animation;
extern raylib::Texture2D red_idle_animation;
/**
 * \brief Total frame count in the idle animation texture.
 */
constexpr std::int8_t kIdleAnimFrameCount = 2;
constexpr float kIdleAnimFrameRate = 1.5f;
extern raylib::Rectangle idle_anim_rec;
extern float idle_anim_frame_counter;

extern raylib::Texture2D blue_spin_animation;
extern raylib::Texture2D red_spin_animation;
/**
 * \brief Total frame count in the spin animation texture.
 */
constexpr std::int8_t kSpinAnimFrameCount = 7; 
constexpr float kSpinAnimFrameRate = 8.f; 
extern raylib::Rectangle spin_anim_rec;
extern float spin_anim_frame_counter;

extern raylib::Texture2D blue_walk_animation;
extern raylib::Texture2D red_walk_animation;
/**
 * \brief Total frame count in the walk animation texture.
 */
constexpr std::int8_t kWalkAnimFrameCount = 6;
constexpr float kWalkAnimFrameRate = 7.f;
extern raylib::Rectangle walk_anim_rec;
extern float walk_anim_frame_counter;

extern raylib::Texture2D blue_hurt_animation;
extern raylib::Texture2D red_hurt_animation;
/**
 * \brief Total frame count in the hurt animation texture.
 */
constexpr std::int8_t kHurtAnimFrameCount = 4;
constexpr float kHurtAnimFrameRate = 8.f;
extern raylib::Rectangle hurt_anim_rec;
extern float hurt_anim_frame_counter;

}
