#pragma once

#include "game_constants.h"
#include "local_game_manager.h"
#include "inputs.h"
#include "types.h"

/**
 * \brief RollbackManager is a class responsible of the integrity of the game
 * simulation.
 *
 * It has 3 game states with different temporalities.
 *
 * The current game state, which is the local client's real-time game.
 *
 * The confirmed state, which is the last game state confirmed by the client.
 * (A checksum is done to confirm the integrity of the simulation.)
 *
 * Finally, the state to be confirmed is the game state calculated by the
 * master client once all the inputs for a frame have been received.
 * All other clients receive the checksum of this game state and verify if it
 * corresponds to their checksum for this given state.
 */
class RollbackManager {
 public:
  void RegisterGameManager(LocalGameManager* current_game_manager) noexcept {
    current_game_manager_ = current_game_manager;
    confirmed_game_manager_.Init(current_game_manager->input_profile_id());
  }

  void SetLocalPlayerInput(input::FrameInput frame_input, PlayerId player_id);
  void SetRemotePlayerInput(const std::vector<input::FrameInput>& frame_inputs,
                            PlayerId player_id);

  void SimulateUntilCurrentFrame() const noexcept;
  Checksum ConfirmFrame() noexcept;

  [[nodiscard]] input::PlayerInput GetLastPlayerConfirmedInput(
      PlayerId player_id) const noexcept;

  [[nodiscard]] FrameNbr current_frame() const noexcept {
    return current_frame_;
  }

  void IncreaseCurrentFrame() noexcept { current_frame_++; }

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
   * \brief current_game_manager_ is a pointer to local client's GameManager.
   */
  LocalGameManager* current_game_manager_ = nullptr;

  /**
   * \brief confirmed_player_manager_ is a copy of the client's player_manager
   * at the last confirmed frame state.
   */
  LocalGameManager confirmed_game_manager_{};

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
   * \brief The frame number of the last confirmed frame (frame verified with
   * checksum).
   */
  FrameNbr confirmed_frame_ = -1;

  /**
   * \brief kMaxFrameCount is the maximum of frame that the game can last.
   * Here 30'000 corresponds to 10 minutes.
   */
  static constexpr FrameNbr kMaxFrameCount = 30'000;

  std::array<std::array<input::PlayerInput, kMaxFrameCount>,
             game_constants::kMaxPlayerCount> inputs_{};

  std::array<input::PlayerInput, 2> last_inputs_{};
};