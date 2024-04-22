#pragma once

#include "World.h"

/**
 * \brief Projectile is a struct containing all the variables that describe
 * the state of a projectile. These variables are the one that are copied
 * when a rollback is needed.
 */
struct Projectile {
  PhysicsEngine::ColliderRef collider_ref{};
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
  void OnTriggerEnter(PhysicsEngine::ColliderRef colliderRefA,
                      PhysicsEngine::ColliderRef colliderRefB) noexcept;

  void Copy(const ProjectileManager& projectile_manager) noexcept;

  [[nodiscard]] std::size_t projectile_count() const noexcept {
    return projectile_count_;
  }
  [[nodiscard]] Math::Vec2F GetProjectilePosition(std::size_t idx) const noexcept;
  [[nodiscard]] Math::CircleF GetProjectileCircle(std::size_t idx) const noexcept;

  static constexpr std::int16_t kMaxProjectileCount = 100;

 private:
  void DestroyProjectile() noexcept;

  static constexpr float kProjectileMoveAmplitude = 7.f;
  static constexpr std::int8_t kMaxCollisionCount = 3;

  std::array<Projectile, kMaxProjectileCount> projectiles_{};
  PhysicsEngine::World* world_ = nullptr;

  std::size_t projectile_count_ = 0;
};
