#include "texture_manager.h"

using namespace raylib;

#include <string_view>

Sprite CreateSprite(std::string_view path, raylib::Vector2 scale,
                    Pivot pivot) {
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

void TextureManager::CreateAllSprites() noexcept {
  ice_ground = CreateSprite("data/images/ice.png", Vector2{2.f, 2.f});
  spikes = CreateSprite("data/images/spkies.png", Vector2{2.f, 2.f});
  log = CreateSprite("data/images/Log.png", Vector2{3.25f, 3.f});
  snow_ball = CreateSprite("data/images/snow_ball.png", Vector2{2.f, 2.f});

  player1_animations.Create("data/images/blue_penguin");
  player2_animations.Create("data/images/red_penguin");
}

void TextureManager::DestroyAllSprites() noexcept {
  UnloadTexture(ice_ground.tex);
  UnloadTexture(spikes.tex);
  UnloadTexture(log.tex);

  player1_animations.Destroy();
  player2_animations.Destroy();
}

void Sprite::Draw(Vector2 position, float rotation, Color color) {
  dest.x = position.x;
  dest.y = position.y;
  DrawTexturePro(tex, source, dest, origin, rotation, color);
}

void PenguinAnimations::Create(std::string_view directory_path) noexcept {
  constexpr float scale_factor = 5.f * 0.8f;

  std::string path{directory_path};
  path += "/Idle.png";
  idle_animation = LoadTexture(path.c_str());
  SetTextureWrap(idle_animation, TEXTURE_WRAP_CLAMP);

  idle_animation.width *= scale_factor;
  idle_animation.height *= scale_factor;

  idle_anim_rec = {
      0.0f, 0.0f,
      static_cast<float>(idle_animation.width) / kIdleAnimFrameCount,
      static_cast<float>(idle_animation.height)};

  path = directory_path;
  path += "/SpinAttack.png";
  spin_animation = LoadTexture(path.c_str());
  SetTextureWrap(spin_animation, TEXTURE_WRAP_CLAMP);

  spin_animation.width *= scale_factor;
  spin_animation.height *= scale_factor;

  spin_anim_rec = {
      0.0f, 0.0f,
      static_cast<float>(spin_animation.width) / kSpinAnimFrameCount,
      static_cast<float>(spin_animation.height)};

  path = directory_path;
  path += "/Walk.png";
  walk_animation = LoadTexture(path.c_str());
  SetTextureWrap(walk_animation, TEXTURE_WRAP_CLAMP);

  walk_animation.width *= scale_factor;
  walk_animation.height *= scale_factor;

  walk_anim_rec = {
      0.0f, 0.0f,
      static_cast<float>(walk_animation.width) / kWalkAnimFrameCount,
      static_cast<float>(walk_animation.height)};

  path = directory_path;
  path += "/Hurt.png";
  hurt_animation = LoadTexture(path.c_str());
  SetTextureWrap(hurt_animation, TEXTURE_WRAP_CLAMP);

  hurt_animation.width *= scale_factor;
  hurt_animation.height *= scale_factor;

  hurt_anim_rec = {
      0.0f, 0.0f,
      static_cast<float>(hurt_animation.width) / kHurtAnimFrameCount,
      static_cast<float>(hurt_animation.height)};
}

void PenguinAnimations::Destroy() noexcept {
  UnloadTexture(idle_animation);
  UnloadTexture(spin_animation);
  UnloadTexture(walk_animation);
  UnloadTexture(hurt_animation);
}
