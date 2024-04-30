#pragma once

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

extern Sprite penguin;
extern Sprite ice_ground;
extern Sprite log;
extern Sprite fire;
}
