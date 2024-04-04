#pragma once

#include <cstdint>

namespace game {

enum class PlayerInputTypes : std::uint8_t {
  kUp = 1 << 0,
  kDown = 1 << 1,
  kRight = 1 << 2,
  kLeft = 1 << 3,
  kShoot = 1 << 4
};

using PlayerInputs = std::uint8_t;

[[nodiscard]] PlayerInputs GetPlayerInputs(int player_idx) noexcept;

}  // namespace game