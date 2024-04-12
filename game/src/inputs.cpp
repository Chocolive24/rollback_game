#include "inputs.h"

#include <raylib.h>

namespace inputs {

PlayerInputs GetPlayerInputs(int input_profile_id) noexcept {
  switch (input_profile_id) {
    case 1: {
      PlayerInputs player_1_inputs = 0;

      if (IsKeyDown(KEY_W)) {
        player_1_inputs |= static_cast<std::uint8_t>(PlayerInputTypes::kUp);
      }
      if (IsKeyDown(KEY_A)) {
        player_1_inputs |= static_cast<std::uint8_t>(PlayerInputTypes::kLeft);
      }
      if (IsKeyDown(KEY_S)) {
        player_1_inputs |= static_cast<std::uint8_t>(PlayerInputTypes::kDown);
      }
      if (IsKeyDown(KEY_D)) {
        player_1_inputs |= static_cast<std::uint8_t>(PlayerInputTypes::kRight);
      }
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        player_1_inputs |= static_cast<std::uint8_t>(PlayerInputTypes::kShoot);
      }
      if (IsKeyPressed(KEY_SPACE)) {
        player_1_inputs |= static_cast<std::uint8_t>(PlayerInputTypes::kJump);
      }
      return player_1_inputs;
    }

    case 2: {
      PlayerInputs player_2_inputs{};

      if (IsKeyDown(KEY_UP)) {
        player_2_inputs |= static_cast<std::uint8_t>(PlayerInputTypes::kUp);
      }
      if (IsKeyDown(KEY_LEFT)) {
        player_2_inputs |= static_cast<std::uint8_t>(PlayerInputTypes::kLeft);
      }
      if (IsKeyDown(KEY_DOWN)) {
        player_2_inputs |= static_cast<std::uint8_t>(PlayerInputTypes::kDown);
      }
      if (IsKeyDown(KEY_RIGHT)) {
        player_2_inputs |= static_cast<std::uint8_t>(PlayerInputTypes::kRight);
      }
      if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        player_2_inputs |= static_cast<std::uint8_t>(PlayerInputTypes::kShoot);
      }
      if (IsKeyPressed(KEY_SPACE)) {
        player_2_inputs |= static_cast<std::uint8_t>(PlayerInputTypes::kJump);
      }
      return player_2_inputs;
    }
    default:
      return PlayerInputs();
  }
}

ClientId::ClientId(std::int8_t id) noexcept { id_ = id; }
}  // namespace input