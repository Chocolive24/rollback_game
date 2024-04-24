#include "inputs.h"

#include <iostream>
#include <raylib.h>

namespace inputs {

PlayerInput GetPlayerInput(int input_profile_id) noexcept {
  switch (input_profile_id) {
    case 0: {
      PlayerInput player_1_inputs = 0;

      if (IsKeyDown(KEY_W)) {
        player_1_inputs |= static_cast<std::uint8_t>(PlayerInputType::kUp);
      }
      if (IsKeyDown(KEY_A)) {
        player_1_inputs |= static_cast<std::uint8_t>(PlayerInputType::kLeft);
      }
      if (IsKeyDown(KEY_S)) {
        player_1_inputs |= static_cast<std::uint8_t>(PlayerInputType::kDown);
      }
      if (IsKeyDown(KEY_D)) {
        player_1_inputs |= static_cast<std::uint8_t>(PlayerInputType::kRight);
      }
      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        // Check if the mouse is not being used for window move.
        if (!IsWindowResized() && !IsWindowMinimized()) {
          player_1_inputs |= static_cast<std::uint8_t>(PlayerInputType::kShoot);
          //std::cout << "input shoot\n";
        }
      }
      if (IsKeyPressed(KEY_SPACE)) {
        player_1_inputs |= static_cast<std::uint8_t>(PlayerInputType::kJump);
      }
      return player_1_inputs;
    }

    case 1: {
      PlayerInput player_2_inputs = 0;

      if (IsKeyDown(KEY_UP)) {
        player_2_inputs |= static_cast<std::uint8_t>(PlayerInputType::kUp);
      }
      if (IsKeyDown(KEY_LEFT)) {
        player_2_inputs |= static_cast<std::uint8_t>(PlayerInputType::kLeft);
      }
      if (IsKeyDown(KEY_DOWN)) {
        player_2_inputs |= static_cast<std::uint8_t>(PlayerInputType::kDown);
      }
      if (IsKeyDown(KEY_RIGHT)) {
        player_2_inputs |= static_cast<std::uint8_t>(PlayerInputType::kRight);
      }
      if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        player_2_inputs |= static_cast<std::uint8_t>(PlayerInputType::kShoot);
      }
      if (IsKeyPressed(KEY_SPACE)) {
        player_2_inputs |= static_cast<std::uint8_t>(PlayerInputType::kJump);
      }
      return player_2_inputs;
    }
    default:
      return PlayerInput();
  }
}

}  // namespace input