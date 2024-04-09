#pragma once

#include <cstdint>

namespace inputs {

enum class PlayerInputTypes : std::uint8_t {
  kUp = 1 << 0,
  kDown = 1 << 1,
  kRight = 1 << 2,
  kLeft = 1 << 3,
  kShoot = 1 << 4
};

/**
 * \brief PlayerInputs is an uint8 which stores all the inputs value of a player
 * in a frame.
 */
using PlayerInputs = std::uint8_t;

[[nodiscard]] PlayerInputs GetPlayerInputs(int player_idx) noexcept;

/**
 * \brief FrameInputs is a struct which contains the inputs off a frame with
 * its number.
 */
struct FrameInputs {
  PlayerInputs inputs = 0;
  std::uint32_t frame_nbr = 0;
};

struct DebugInputs {
  FrameInputs frame_inputs{};
  float delay = 0.f;
  int client_idx = 0;
};

}  // namespace inputs