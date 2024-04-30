#include "texture_manager.h"

using namespace raylib;

#include <string_view>

namespace texture_manager {

 Sprite penguin{};
 Sprite ice_ground{};
 Sprite log{};
 Sprite fire{};

Sprite CreateSprite(std::string_view path, Vector2 scale,
                    Pivot pivot = Pivot::kCenter) {
  Sprite sprite{};
  sprite.tex = LoadTexture(path.data());
  sprite.source = {0, 0, static_cast<float>(sprite.tex.width),
                   static_cast<float>(sprite.tex.height)};

  switch (pivot) {
    case Pivot::kCenter:
      sprite.origin = {sprite.tex.width * scale.x * 0.5f,
                       sprite.tex.height * scale.y * 0.5f};
      break;
    case Pivot::kTopLeft:
      sprite.origin = {0, 0};
      break;
    case Pivot::kTopRight:
      sprite.origin = {sprite.tex.width * scale.x, 0};
      break;
    case Pivot::kDownLeft:
      sprite.origin = {0, sprite.tex.height * scale.y};
      break;
    case Pivot::kDownRight:
      sprite.origin = {sprite.tex.width * scale.x, sprite.tex.height * scale.y};
      break;
    case Pivot::kDownCenter:
      sprite.origin = {sprite.tex.width * scale.x * 0.5f,
                       sprite.tex.height * scale.y};
      break;
    case Pivot::kBackGround:
      break;
    default:
      break;
  }

  sprite.dest = {0, 0, sprite.source.width * scale.x,
                 sprite.source.height * scale.y};

  return sprite;
}

void CreateAllSprites() noexcept {
  ice_ground = CreateSprite("data/images/ice.png", Vector2{2.f, 2.f});
  fire = CreateSprite("data/images/fire.png", Vector2{4.f, 4.f});
  log = CreateSprite("data/images/Log.png", Vector2{3.25f, 3.f});
  penguin =
      CreateSprite("data/images/Turn.png", Vector2{5.f * 0.75f, 5.f * 0.75f});

}

void DestroyAllSprites() noexcept { UnloadTexture(penguin.tex); }

}  // namespace texture_manager

void Sprite::Draw(Vector2 position, float rotation, Color color) {
  dest.x = position.x;
  dest.y = position.y;
  DrawTexturePro(tex, source, dest, origin, rotation, color);
}
