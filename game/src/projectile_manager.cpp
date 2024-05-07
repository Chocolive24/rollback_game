#include "projectile_manager.h"

#include "game_constants.h"

#include <iostream>
#include <algorithm>

void ProjectileManager::Init(PhysicsEngine::World* world) noexcept {
  world_ = world;

  for (std::size_t i = 0; i < kMaxProjectileCount; i++)
  {
    const auto& body_ref = world_->CreateBody();
    auto& body = world_->GetBody(body_ref);
    body.SetBodyType(PhysicsEngine::BodyType::Kinematic);
    body.SetMass(kProjectileMass);

    projectiles_[i].collider_ref = world_->CreateCollider(body_ref);
    auto& collider = world_->GetCollider(projectiles_[i].collider_ref);
    collider.SetShape(Math::CircleF(Math::Vec2F::Zero(), kProjectileRadius));
    collider.SetRestitution(1.f);
    collider.SetEnabled(false);
  }
}

void ProjectileManager::CreateProjectile(Math::Vec2F position,
                                         Math::Vec2F mov_dir) noexcept {
  // Find the first projectile not enabled.
  const auto& proj_it = std::find_if(
      projectiles_.begin(), projectiles_.end(),
               [this](const Projectile& proj) {
                 return !world_->GetCollider(proj.collider_ref).Enabled();
               });

  // If all projectiles in the pool are already used, return.
  if (proj_it == projectiles_.end()) {
    std::cerr << "No more space to create a projectile. \n";
    return;
  }

  // Reset the projectile state and set its position and velocity.
  auto& proj = *proj_it;
  proj.collision_count = 0;

  auto& collider = world_->GetCollider(proj.collider_ref);
  collider.SetEnabled(true);

  auto& body = world_->GetBody(collider.GetBodyRef());
  body.SetPosition(position);
  body.SetVelocity(mov_dir * kProjectileMoveAmplitude);
  body.SetMass(kProjectileMass);
}

void ProjectileManager::FixedUpdate() noexcept {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif  // TRACY_ENABLE

  for (auto& proj : projectiles_) {
    auto& collider = world_->GetCollider(proj.collider_ref);
    if (!collider.Enabled()) 
        continue;

    if (proj.collision_count >= kMaxCollisionCount)
    {
        collider.SetEnabled(false);
    }
  }
}

void ProjectileManager::Deinit() noexcept {
  projectiles_.fill(Projectile());
}


void ProjectileManager::OnCollisionEnter(
    PhysicsEngine::ColliderRef colliderRefA,
    PhysicsEngine::ColliderRef colliderRefB) noexcept {
  for (auto& proj : projectiles_) {
    auto& collider = world_->GetCollider(proj.collider_ref);

    if (!collider.Enabled()) 
        continue;

    if (proj.collider_ref == colliderRefA || proj.collider_ref == colliderRefB) {
      proj.collision_count++;
    }
  }
}

Checksum ProjectileManager::ComputeChecksum() const noexcept {
  Checksum checksum = 0;

  for (const auto& proj : projectiles_) {
    const auto& body_ref = world_->GetCollider(proj.collider_ref).GetBodyRef();
    const auto& body = world_->GetBody(body_ref);

    // Add position.
    const auto& pos = body.Position();
    const auto* pos_ptr = reinterpret_cast<const Checksum*>(&pos);
    for (size_t i = 0; i < sizeof(Math::Vec2F) / sizeof(Checksum); i++) {
      checksum += pos_ptr[i];
    }

    // Add velocity.
    const auto& velocity = body.Velocity();
    const auto* velocity_ptr = reinterpret_cast<const Checksum*>(&velocity);
    for (size_t i = 0; i < sizeof(Math::Vec2F) / sizeof(Checksum); i++) {
      checksum += velocity_ptr[i];
    }

    // Add collision count.
    checksum += proj.collision_count;

    // Add is enabled.
    const auto& collider = world_->GetCollider(proj.collider_ref);
    const auto& is_enabled = collider.Enabled();
    checksum += is_enabled ? 1 : 0;
  }

  return checksum;
}

void ProjectileManager::Rollback(const ProjectileManager& projectile_manager) noexcept {
  projectiles_ = projectile_manager.projectiles_;
}

Math::Vec2F ProjectileManager::GetProjectilePosition(std::size_t idx) const noexcept {
  const auto& body_ref =
      world_->GetCollider(projectiles_[idx].collider_ref).GetBodyRef();
  const auto& body = world_->GetBody(body_ref);

  return body.Position();
}

Math::Vec2F ProjectileManager::GetProjectileVelocity(
    std::size_t idx) const noexcept {
  const auto& body_ref =
      world_->GetCollider(projectiles_[idx].collider_ref).GetBodyRef();
  const auto& body = world_->GetBody(body_ref);

  return body.Velocity();
}

Math::CircleF ProjectileManager::GetProjectileCircle(std::size_t idx) const noexcept {
  const auto& collider = world_->GetCollider(projectiles_[idx].collider_ref);

  return std::get<Math::CircleF>(collider.Shape());
}