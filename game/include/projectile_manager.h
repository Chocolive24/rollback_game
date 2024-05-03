#pragma once

#include "types.h"
#include "World.h"

/**
 * \brief Projectile is a struct containing all the variables that describe
 * the state of a projectile. These variables are the one that are copied
 * when a rollback is needed.
 */
struct Projectile {
  PhysicsEngine::ColliderRef collider_ref{};
  float rotation = 0.f;
  std::int16_t collision_count = 0;
};

/**
 * \brief ProjectileManager is a class which is responsible to create/destroy and
 * store all projectiles of the game.
 *
 * It has a pointer to the world to do so.
 */
class ProjectileManager {
public:
  void Init(PhysicsEngine::World* world) noexcept;
  void CreateProjectile(Math::Vec2F position, Math::Vec2F mov_dir) noexcept;
  void FixedUpdate() noexcept;
  void OnCollisionEnter(PhysicsEngine::ColliderRef colliderRefA,
                      PhysicsEngine::ColliderRef colliderRefB) noexcept;

  Checksum ComputeChecksum() const noexcept;
  void Rollback(const ProjectileManager& projectile_manager) noexcept;


  [[nodiscard]] const Projectile& GetProjectile(std::size_t idx) const noexcept {
    return projectiles_[idx];
  }

  [[nodiscard]] Math::Vec2F GetProjectilePosition(std::size_t idx) const noexcept;
  [[nodiscard]] Math::Vec2F GetProjectileVelocity(std::size_t idx) const noexcept;
  [[nodiscard]] Math::CircleF GetProjectileCircle(std::size_t idx) const noexcept;

  [[nodiscard]] bool IsProjectileEnabled(std::size_t idx) const noexcept {
    return world_->GetCollider(projectiles_[idx].collider_ref).Enabled();
  }

  static constexpr std::int16_t kMaxProjectileCount = 100;

 private:
  static constexpr float kProjectileMoveAmplitude = 7.f;
  static constexpr std::int8_t kMaxCollisionCount = 4;
  static constexpr float kProjectileMass = 2.5f;
  static constexpr float kProjectileRadius = 0.125f;

  std::array<Projectile, kMaxProjectileCount> projectiles_{};
  PhysicsEngine::World* world_ = nullptr;
};
