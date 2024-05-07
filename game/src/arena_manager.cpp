#include "arena_manager.h"

void ArenaManager::Init(PhysicsEngine::World* world) noexcept {
  world_ = world;

  for (int i = 0; i < game_constants::kArenaBorderWallCount; i++) {
    const auto& body_ref = world_->CreateBody();
    auto& body = world_->GetBody(body_ref);
    body.SetPosition(border_wall_positions[i]);
    body.SetBodyType(PhysicsEngine::BodyType::Static);

    wall_col_refs[i] = world_->CreateCollider(body_ref);
    auto& col = world_->GetCollider(wall_col_refs[i]);
    col.SetShape(wall_shapes[i]);
    col.SetRestitution(0.f);
  }

  for (int i = 0; i < game_constants::kArenaSquareWallCount; i++) {
    const auto idx = i + game_constants::kArenaBorderWallCount;
    const auto& body_ref = world_->CreateBody();
    auto& body = world_->GetBody(body_ref);
    body.SetPosition(square_wall_positions[i]);
    body.SetBodyType(PhysicsEngine::BodyType::Static);

    wall_col_refs[idx] = world_->CreateCollider(body_ref);
    auto& col = world_->GetCollider(wall_col_refs[idx]);
    col.SetShape(game_constants::kSquareWallRect);
    col.SetRestitution(0.f);
  }
}