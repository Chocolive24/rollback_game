#include "projectile_manager.h"

#include <cassert>
#include <iostream>

void ProjectileManager::Init(PhysicsEngine::World* world) noexcept {
  world_ = world;

  for (std::size_t i = 0; i < 100; i++)
  {
    const auto& body_ref = world_->CreateBody();
    auto& body = world_->GetBody(body_ref);
    body.SetBodyType(PhysicsEngine::BodyType::Kinematic);
    body.SetMass(5.f);

    projectiles_[i].collider_ref = world_->CreateCollider(body_ref);
    auto& collider = world_->GetCollider(projectiles_[i].collider_ref);
    collider.SetShape(Math::CircleF(Math::Vec2F::Zero(), 0.2f));
    collider.SetRestitution(1.f);
    collider.SetEnabled(false);
  }
}

void ProjectileManager::CreateProjectile(Math::Vec2F position,
                                         Math::Vec2F mov_dir) noexcept {
  if (projectile_count_ >= kMaxProjectileCount)
  {
    std::cerr << "No more space to create a projectile. \n";
    return;
  }

  auto& collider = world_->GetCollider(projectiles_[projectile_count_].collider_ref);
  collider.SetEnabled(true);

  auto& body = world_->GetBody(collider.GetBodyRef());
  body.SetPosition(position);
  body.SetVelocity(mov_dir * kProjectileMoveAmplitude);

  projectile_count_++;
}

void ProjectileManager::OnTriggerEnter(
    PhysicsEngine::ColliderRef colliderRefA,
    PhysicsEngine::ColliderRef colliderRefB) noexcept {
  //TODO: increase collider count and disable collider if the max collision count is reached.
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

    // Add is enabled.
    const auto& collider = world_->GetCollider(proj.collider_ref);
    const auto& is_enabled = collider.Enabled();
    checksum += is_enabled ? 1 : 0;
  }

  return checksum;
}

void ProjectileManager::Copy(const ProjectileManager& projectile_manager) noexcept {
  projectiles_ = projectile_manager.projectiles_;
  projectile_count_ = projectile_manager.projectile_count_;
}

Math::Vec2F ProjectileManager::GetProjectilePosition(std::size_t idx) const noexcept {
  const auto& body_ref =
      world_->GetCollider(projectiles_[idx].collider_ref).GetBodyRef();
  const auto& body = world_->GetBody(body_ref);

  return body.Position();
}

Math::CircleF ProjectileManager::GetProjectileCircle(std::size_t idx) const noexcept {
  const auto& collider = world_->GetCollider(projectiles_[idx].collider_ref);

  return std::get<Math::CircleF>(collider.Shape());
}

void ProjectileManager::DestroyProjectile() noexcept {
  for (std::size_t i = 0; i < kMaxProjectileCount; i++) {
    projectiles_[i] = projectiles_[projectile_count_ - 1];
    projectile_count_--;
    i--;
  }
}
