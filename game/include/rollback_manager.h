#pragma once

#include "types.h"
#include "game_constants.h"
#include "inputs.h"
#include "game_manager.h"

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
  void RegisterGameManager(GameManager* player_manager) noexcept {
    current_game_manager_ = player_manager;
    confirmed_game_manager_ = *player_manager;
  }

  void SetLocalPlayerInput(inputs::FrameInput frame_input, PlayerId player_id);
  void SetRemotePlayerInput(const std::vector<inputs::FrameInput>& frame_inputs, 
                            PlayerId player_id);

  void SimulateUntilCurrentFrame() noexcept;
  [[nodiscard]] int ComputeFrameToConfirmChecksum() noexcept;
  void ConfirmFrame(FrameNbr confirm_frame) noexcept;

  [[nodiscard]] FrameNbr confirmed_frame() const noexcept {
    return confirmed_frame_;
  }

  [[nodiscard]] FrameNbr last_remote_input_frame() const noexcept {
    return last_remote_input_frame_;
  }

  [[nodiscard]] FrameNbr frame_to_confirm() const noexcept {
    return frame_to_confirm_;
  }

private:
  /**
   * \brief current_play_manager_ is a pointer to client's player_manager
   */
  GameManager* current_game_manager_ = nullptr;

  /**
   * \brief confirmed_player_manager_ is a copy of the client's player_manager
   * at the last confirmed frame state.
   */
  GameManager confirmed_game_manager_{};

  /**
   * \brief player_manager_to_confirm_ is the state of the player_manager at the
   * frame to confirm state.
   */
  GameManager player_manager_to_confirm_{};

  std::vector<GameManager> game_managers_{};

  /**
   * \brief The frame nbr of the local client.
   */
  FrameNbr current_frame_ = -1;

  /**
   * \brief The frame number of the last time a remote input was received.
   */
  FrameNbr last_remote_input_frame_ = -1;

  /**
   * \brief The frame number which the master client wants to confirm.
   */
  FrameNbr frame_to_confirm_ = 0;

  /**
   * \brief The frame number of the last confirmed frame (frame verified with checksum).
   */
  FrameNbr confirmed_frame_ = -1;

  std::array<std::array<inputs::PlayerInput, 30000>,
             game_constants::kMaxPlayerCount> inputs_{};
};