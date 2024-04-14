#include "platforms_manager.h"

void PlatformManager::Init(PhysicsEngine::World* world) noexcept {
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
  col.SetRestitution(0.f);
}

Math::Vec2F PlatformManager::GetPlatformPosition(const std::size_t& idx) const noexcept {
  const auto& body_ref =
      world_->GetCollider(platform_col_refs[idx]).GetBodyRef();
  return world_->GetBody(body_ref).Position();
}

Math::RectangleF PlatformManager::GetPlatformShape(
    const std::size_t& idx) const noexcept {
  const auto& col = world_->GetCollider(platform_col_refs[idx]);
  return std::get<Math::RectangleF>(col.Shape());
}
