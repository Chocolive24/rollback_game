#pragma once

#include <cstdint>

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

[[nodiscard]] PlayerInput GetPlayerInput(int input_profile_id) noexcept;

/**
 * \brief FrameInputs is a struct which contains the inputs off a frame with
 * its number.
 */
struct FrameInput {
  PlayerInput input = 0;
  short frame_nbr = 0;

};

/**
 * \brief ClientId 
 */
class ClientId {
public:
  explicit ClientId(std::int8_t id) noexcept;

  //TODO operator de conversion vers int.

 private:
  std::int8_t id_ = 0;
};

struct DebugInput {
  FrameInput frame_inputs{};
  float delay = 0.f;
  int client_idx = 0;
};

}  // namespace input