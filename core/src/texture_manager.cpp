#include "texture_manager.h"

using namespace raylib;

#include <string_view>

namespace texture_manager {

 Sprite penguin_blue{};
 Sprite penguin_red{};
 Sprite ice_ground{};
 Sprite log{};
 Sprite spikes{};

 raylib::Texture2D blue_idle_animation{};
 raylib::Texture2D red_idle_animation{};
 raylib::Rectangle idle_anim_rec;
 float idle_anim_frame_counter;

 raylib::Texture2D blue_spin_animation{};
 raylib::Texture2D red_spin_animation{};
 raylib::Rectangle spin_anim_rec;
 float spin_anim_frame_counter;

 raylib::Texture2D blue_walk_animation{};
 raylib::Texture2D red_walk_animation{};
 raylib::Rectangle walk_anim_rec;
 float walk_anim_frame_counter;

 raylib::Texture2D blue_hurt_animation{};
 raylib::Texture2D red_hurt_animation{};
 raylib::Rectangle hurt_anim_rec;
 float hurt_anim_frame_counter;

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

  constexpr float scale_factor = 5.f * 0.8f;

  blue_idle_animation = LoadTexture("data/images/blue_penguin/Idle.png");
  SetTextureWrap(blue_idle_animation, TEXTURE_WRAP_CLAMP);

  blue_idle_animation.width *= scale_factor;
  blue_idle_animation.height *= scale_factor;

  red_idle_animation = LoadTexture("data/images/red_penguin/Idle.png");
  SetTextureWrap(red_idle_animation, TEXTURE_WRAP_CLAMP);

  red_idle_animation.width *= scale_factor;
  red_idle_animation.height *= scale_factor;

  idle_anim_rec = {
      0.0f, 0.0f,
      static_cast<float>(blue_idle_animation.width) / kIdleAnimFrameCount,
      static_cast<float>(blue_idle_animation.height)};

  blue_spin_animation = LoadTexture("data/images/blue_penguin/SpinAttack.png");
  SetTextureWrap(blue_spin_animation, TEXTURE_WRAP_CLAMP);

  blue_spin_animation.width *= scale_factor;
  blue_spin_animation.height *= scale_factor;

  red_spin_animation = LoadTexture("data/images/red_penguin/SpinAttack.png");
  SetTextureWrap(red_spin_animation, TEXTURE_WRAP_CLAMP);

  red_spin_animation.width *= scale_factor;
  red_spin_animation.height *= scale_factor;

  spin_anim_rec = {
      0.0f, 0.0f,
      static_cast<float>(blue_spin_animation.width) / kSpinAnimFrameCount,
      static_cast<float>(blue_spin_animation.height)};

  blue_walk_animation = LoadTexture("data/images/blue_penguin/Walk.png");
  SetTextureWrap(blue_walk_animation, TEXTURE_WRAP_CLAMP);

  blue_walk_animation.width *= scale_factor;
  blue_walk_animation.height *= scale_factor;

  red_walk_animation = LoadTexture("data/images/red_penguin/Walk.png");
  SetTextureWrap(red_walk_animation, TEXTURE_WRAP_CLAMP);

  red_walk_animation.width *= scale_factor;
  red_walk_animation.height *= scale_factor;

  walk_anim_rec = {
      0.0f, 0.0f,
      static_cast<float>(blue_walk_animation.width) / kWalkAnimFrameCount,
      static_cast<float>(blue_walk_animation.height)};

  blue_hurt_animation = LoadTexture("data/images/blue_penguin/Hurt.png");
  SetTextureWrap(blue_hurt_animation, TEXTURE_WRAP_CLAMP);

  blue_hurt_animation.width *= scale_factor;
  blue_hurt_animation.height *= scale_factor;

  red_hurt_animation = LoadTexture("data/images/red_penguin/Hurt.png");
  SetTextureWrap(red_hurt_animation, TEXTURE_WRAP_CLAMP);

  red_hurt_animation.width *= scale_factor;
  red_hurt_animation.height *= scale_factor;

  hurt_anim_rec = {
      0.0f, 0.0f,
      static_cast<float>(blue_hurt_animation.width) / kHurtAnimFrameCount,
      static_cast<float>(blue_hurt_animation.height)};
}

void DestroyAllSprites() noexcept { UnloadTexture(penguin_blue.tex); }

}  // namespace texture_manager

void Sprite::Draw(Vector2 position, float rotation, Color color) {
  dest.x = position.x;
  dest.y = position.y;
  DrawTexturePro(tex, source, dest, origin, rotation, color);
}
