#pragma once

#include "types.h"
#include "game_constants.h"
#include "inputs.h"
#include "player_controller.h"

class GameManager;
class NetworkInterface;

/**
 * \brief RollbackManager is a class responsible for the integrity of the game simulation.
 *
 * It has 3 game states with different temporalities.
 *
 * The current game state, which is the local client's real-time game.
 *
 * The confirmed state, which is the last game state confirmed by the master client
 * when all clients have the same simulation.
 *
 * Finally, the state to be confirmed, which is the game state calculated by the
 * master client once all the inputs for a frame have been received.
 * This state becomes the confirmed state once a validation of the simulation's
 * integrity has been received by the other clients.
 */
class RollbackManager {
public:
  void RegisterPlayerManager(PlayerManager* player_manager) noexcept {
    current_player_manager_ = player_manager;
    confirmed_player_manager = *player_manager;
  }

  void SetPlayerInput(inputs::FrameInput frame_input, PlayerId player_id);
  void SetRemotePlayerInput(inputs::FrameInput frame_input, PlayerId player_id);

  void SimulateUntilCurrentFrame() noexcept;

  void SetCurrentFrame(FrameNbr current_frame) noexcept {
    current_frame_ = current_frame;
  }

  uint32_t SimulateUntilFrameToConfirm(FrameNbr frame_to_confirm) noexcept;
  void ConfirmFrame(FrameNbr confirm_frame) noexcept;

  [[nodiscard]] FrameNbr confirmed_frame() const noexcept {
    return confirmed_frame_;
  }

private:
  /**
   * \brief current_play_manager_ is a pointer to client's player_manager
   */
  PlayerManager* current_player_manager_ = nullptr;

  /**
   * \brief confirmed_player_manager is a copy of the client's player_manager
   * at the last confirmed frame state.
   */
  PlayerManager confirmed_player_manager{};

  /**
   * \brief player_manager_to_confirm_ is the last state calculated by the master
   * client after receiving all inputs for a frame.
   * This state is stored until the master client receives confirmation that all
   * clients have the same simulation.
   */
  PlayerManager player_manager_to_confirm_{};

  // The frame nbr of the local client.
  FrameNbr current_frame_ = -1;

  /**
   * \brief The frame number of the last time a remote input was received.
   */
  FrameNbr last_remote_input_frame_ = -1;

  /**
   * \brief The frame number which the master client wants to confirm.
   */
  FrameNbr frame_to_confirm_ = -1;

  // The frame number of the last confirmed frame (frame verified with checksum).
  FrameNbr confirmed_frame_ = -1;

  std::array<std::array<inputs::FrameInput, 20000>,
             game_constants::kMaxPlayerCount> inputs_{};
};

