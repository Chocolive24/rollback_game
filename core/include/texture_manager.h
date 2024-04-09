#pragma once

#include <raylib.h>
#include <raymath.h>

#include <cstdint>

enum class Pivot : std::uint8_t {
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
  Texture2D tex;
  Vector2 pos;
  Vector2 origin;
  Rectangle source;
  Rectangle dest;

  void Draw(Vector2 position, float rotation = 0, Color color = WHITE);
};

namespace texture_manager {

void CreateAllSprites() noexcept;
void DestroyAllSprites() noexcept;

inline Sprite penguin{};
}
