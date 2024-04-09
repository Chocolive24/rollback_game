#include "platforms_manager.h"

void PlatformsManager::Init(PhysicsEngine::World* world) noexcept {
  world_ = world;

  const auto& body_ref = world_->CreateBody();
  auto& body = world_->GetBody(body_ref);
  body.SetPosition(game_constants::kMainPlatformPos);
  body.SetBodyType(PhysicsEngine::BodyType::Static);
  platform_col_refs[0] = world_->CreateCollider(body_ref);
  auto& col = world_->GetCollider(platform_col_refs[0]);
  constexpr auto kMainPlatColHalfSize =
      game_constants::kMainPlatformColSize * 0.5f;
  col.SetShape(Math::RectangleF(-kMainPlatColHalfSize, kMainPlatColHalfSize));
  col.SetRestitution(1.f);
}
