#pragma once

#include <vector>

#include "types.h"

namespace inputs {

enum class PlayerInputType : std::uint8_t {
  kUp = 1 << 0,
  kDown = 1 << 1,
  kRight = 1 << 2,
  kLeft = 1 << 3,
  kShoot = 1 << 4,
  kJump = 1 << 5
};

/**
 * \brief PlayerInputs is an uint8 which stores all the inputs value of a player
 * in a frame.
 */
using PlayerInput = std::uint8_t;

  constexpr PlayerInput kInvalidInput = 255;

[[nodiscard]] PlayerInput GetPlayerInput(int input_profile_id) noexcept;

/**
 * \brief FrameInputs is a struct which contains the inputs off a frame with
 * its number.
 */
struct FrameInput {
  PlayerInput input = 0;
  FrameNbr frame_nbr = 0;
};

struct FrameToConfirm {
  int checksum;
  std::vector<FrameInput> frame_inputs{};
};

/**
 * \brief SimulationInput is a struct containing a frame input and have a delay value
 * to simulate the network delay.
 */
struct SimulationInput {
  std::vector<FrameInput> frame_inputs;
  float delay = 0.f;
};

//struct SimulationInput {
//  FrameInput frame_input;
//  float delay = 0.f;
//};

struct DebugInput {
  FrameInput frame_inputs{};
  float delay = 0.f;
  int client_idx = 0;
};

}  // namespace input