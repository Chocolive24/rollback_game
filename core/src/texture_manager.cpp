#include "texture_manager.h"

using namespace raylib;

#include <string_view>

namespace texture_manager {

 Sprite penguin_blue{};
 Sprite penguin_red{};
 Sprite ice_ground{};
 Sprite log{};
 Sprite spikes{};

 raylib::Texture2D spin_animation{};
 raylib::Rectangle spin_anim_rec;
 float spin_anim_frame_counter;

 raylib::Texture2D walk_animation{};
 raylib::Rectangle walk_anim_rec;
 float walk_anim_frame_counter;

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
  spikes = CreateSprite("data/images/spkies.png", Vector2{2.f, 2.f});
  log = CreateSprite("data/images/Log.png", Vector2{3.25f, 3.f});
  penguin_blue =
      CreateSprite("data/images/penguin_blue.png", Vector2{5.f * 0.8f, 5.f * 0.8f});
  penguin_red = CreateSprite("data/images/penguin_red.png",
                              Vector2{5.f * 0.8f, 5.f * 0.8f});

  constexpr float scale_factor = 5.f * 0.8f;

  spin_animation = LoadTexture("data/images/SpinAttack.png");
  SetTextureWrap(spin_animation, TEXTURE_WRAP_CLAMP);

  spin_animation.width *= scale_factor;
  spin_animation.height *= scale_factor;

  spin_anim_rec = {
      0.0f, 0.0f,
      static_cast<float>(spin_animation.width) / kSpinAnimFrameCount,
      static_cast<float>(spin_animation.height)};

  walk_animation = LoadTexture("data/images/Walk.png");
  SetTextureWrap(walk_animation, TEXTURE_WRAP_CLAMP);

  walk_animation.width *= scale_factor;
  walk_animation.height *= scale_factor;

  walk_anim_rec = {
      0.0f, 0.0f,
      static_cast<float>(walk_animation.width) / kWalkAnimFrameCount,
      static_cast<float>(walk_animation.height)};
}

void DestroyAllSprites() noexcept { UnloadTexture(penguin_blue.tex); }

}  // namespace texture_manager

void Sprite::Draw(Vector2 position, float rotation, Color color) {
  dest.x = position.x;
  dest.y = position.y;
  DrawTexturePro(tex, source, dest, origin, rotation, color);
}
