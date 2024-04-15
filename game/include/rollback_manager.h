#pragma once

#include <vector>

#include "types.h"
#include "game_constants.h"
#include "inputs.h"
#include "player_controller.h"

class GameManager;

class RollbackManager {
public:
  void RegisterPlayerManager(PlayerManager* player_manager) noexcept {
    current_player_manager_ = player_manager;
    confirmed_player_manager = *player_manager;
  }

  [[nodiscard]] inputs::PlayerInput GetPlayerInputAtFrame(
      FrameNbr frame_nbr, PlayerId player_id) noexcept;

  void SetPlayerInput(inputs::FrameInput frame_input, PlayerId player_id);
  void SetRemotePlayerInput(inputs::FrameInput frame_input, PlayerId player_id);

  void SimulateUntilCurrentFrame() noexcept;

  void SetCurrentFrame(FrameNbr current_frame) noexcept {
    current_frame_ = current_frame;
  }

private:
//TODO: pour l'instant, on part du principe que les inputs arrive dans l'ordre sans pertes.
  //TODO: Donc on est au max en retard d' 1 input/frame.

  GameManager* game_manager_ = nullptr;

  /**
   * \brief current_play_manager_ is a pointer to client's player_manager
   */
  PlayerManager* current_player_manager_ = nullptr;

  /**
   * \brief confirmed_player_manager is a copy of the client's player_manager
   * at the last confirmed frame state.
   */
  PlayerManager confirmed_player_manager{};

  // The frame nbr of the client.
  FrameNbr current_frame_ = -1;

  // The frame number of the last time a remote input was received.
  FrameNbr last_remote_input_frame = -1;

  //std::array<std::array<inputs::PlayerInput, 32767>,
  //  game_constants::kMaxPlayerCount> inputs_{};

  //std::array<std::vector<inputs::PlayerInput>,
  //           game_constants::kMaxPlayerCount>
  //    inputs_{};

    std::array<std::array<inputs::PlayerInput, 63000>, game_constants::kMaxPlayerCount>
      inputs_{};
};

