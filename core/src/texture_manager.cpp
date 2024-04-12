#include "texture_manager.h"

using namespace raylib;

#include <string_view>

namespace texture_manager {

 Sprite penguin{};

Sprite CreateSprite(std::string_view path, float scale,
                    Pivot pivot = Pivot::kCenter) {
  Sprite sprite{};
  sprite.tex = LoadTexture(path.data());
  sprite.source = {0, 0, static_cast<float>(sprite.tex.width),
                   static_cast<float>(sprite.tex.height)};

  switch (pivot) {
    case Pivot::kCenter:
      sprite.origin = {sprite.tex.width * scale * 0.5f,
                       sprite.tex.height * scale * 0.5f};
      break;
    case Pivot::kTopLeft:
      sprite.origin = {0, 0};
      break;
    case Pivot::kTopRight:
      sprite.origin = {sprite.tex.width * scale, 0};
      break;
    case Pivot::kDownLeft:
      sprite.origin = {0, sprite.tex.height * scale};
      break;
    case Pivot::kDownRight:
      sprite.origin = {sprite.tex.width * scale, sprite.tex.height * scale};
      break;
    case Pivot::kDownCenter:
      sprite.origin = {sprite.tex.width * scale * 0.5f,
                       sprite.tex.height * scale};
      break;
    case Pivot::kBackGround:
      break;
    default:
      break;
  }

  sprite.dest = {0, 0, sprite.source.width * scale,
                 sprite.source.height * scale};

  return sprite;
}

void CreateAllSprites() noexcept {
  penguin = CreateSprite("data/images/Turn.png", 5.f);
}

void DestroyAllSprites() noexcept { UnloadTexture(penguin.tex); }

}  // namespace texture_manager

void Sprite::Draw(Vector2 position, float rotation, Color color) {
  dest.x = position.x;
  dest.y = position.y;
  DrawTexturePro(tex, source, dest, origin, rotation, color);
}
