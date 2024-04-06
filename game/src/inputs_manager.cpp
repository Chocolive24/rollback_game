#include "inputs_manager.h"

#include <raylib.h>

namespace inputs {

PlayerInputs GetPlayerInputs(int player_idx) noexcept {
  switch (player_idx) {
    case 1: {
      PlayerInputs player_1_inputs{};

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

      return player_2_inputs;
    }
    default:
      return PlayerInputs();
  }
}

}  // namespace inputs