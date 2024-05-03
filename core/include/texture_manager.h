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

extern raylib::Texture2D spin_animation;
/**
 * \brief Total frame count in the spin animation texture.
 */
constexpr std::int8_t kSpinAnimFrameCount = 7; 
extern raylib::Rectangle spin_anim_rec;
extern float spin_anim_frame_counter;

extern raylib::Texture2D walk_animation;
/**
 * \brief Total frame count in the spin animation texture.
 */
constexpr std::int8_t kWalkAnimFrameCount = 6;
extern raylib::Rectangle walk_anim_rec;
extern float walk_anim_frame_counter;

}
