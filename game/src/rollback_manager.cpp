#include "rollback_manager.h"

#include <iostream>

#include "game_manager.h"

void RollbackManager::SetLocalPlayerInput(inputs::FrameInput frame_input,
                                          PlayerId player_id) {
  inputs_[player_id][frame_input.frame_nbr] = frame_input.input;
  current_game_manager_->SetPlayerInput(frame_input.input, player_id);
  current_frame_ = frame_input.frame_nbr;
}

void RollbackManager::SetRemotePlayerInput(
  const std::vector<inputs::FrameInput>& frame_inputs, PlayerId player_id) {

  auto last_remote_frame_input = frame_inputs.back();
  const auto frame_diff =
      last_remote_frame_input.frame_nbr - last_remote_input_frame_;

  // If the last remote input is greater than the local current frame, set the
  // last_remote_frame_input value with the remote input value for my current_frame to
  // avoid the confirmation of a future frame without the local inputs.
  if (last_remote_frame_input.frame_nbr > current_frame_)
  {
    const auto& current_frame_it =
        std::find_if(frame_inputs.begin(), frame_inputs.end(),
                     [this](inputs::FrameInput frame_input) {
                       return frame_input.frame_nbr == current_frame_;
                     });

    last_remote_frame_input = *current_frame_it;
  }


  // The inputs are already received.
  if (frame_diff < 1)
  {
    return;
  }

  // If we didn't receive some inputs between the last time and the new inputs,
  // iterates over the missing inputs to add them in the inputs array.
  const auto it = std::find_if(frame_inputs.begin(), frame_inputs.end(),
                               [this](inputs::FrameInput frame_input) {
                                 return frame_input.frame_nbr ==
                                        last_remote_input_frame_ + 1;
                               });

  auto idx = std::distance(frame_inputs.begin(), it);
  for (int i = last_remote_input_frame_ + 1; i <= last_remote_frame_input.frame_nbr; i++) {
    const auto input = frame_inputs[idx].input;
    inputs_[player_id][i] = input;
    idx++;
  }

  // Predicts the remote client would not change its inputs until the current
  // frame to have a realtime simulation.
  //for (FrameNbr frame = latest_frame_input.frame_nbr; frame <= current_frame_;
  //     frame++) {
  //  inputs_[player_id][frame] = latest_frame_input.input;
  //}

  SimulateUntilCurrentFrame();

  last_remote_input_frame_ = last_remote_frame_input.frame_nbr;
}

void RollbackManager::SimulateUntilCurrentFrame() noexcept {
  current_game_manager_->Copy(confirmed_game_manager_);

  for (int i = confirmed_frame_ + 1; i < current_frame_; i++) {
    for (PlayerId player_id = 0; player_id < game_constants::kMaxPlayerCount;
         player_id++) {
      const auto input = inputs_[player_id][i];
      current_game_manager_->SetPlayerInput(input, player_id);
    }

    current_game_manager_->FixedUpdate();
  }

  // The Fixed update of the current frame is made in the main loop after polling
  // received events from network.
}

int RollbackManager::ComputeFrameToConfirmChecksum() noexcept {
  game_manager_to_confirm_.Copy(confirmed_game_manager_);

  for (int frame = confirmed_frame_ + 1; frame <= frame_to_confirm_; frame++) {
    for (PlayerId player_id = 0; player_id < game_constants::kMaxPlayerCount;
         player_id++) {
      const auto input = inputs_[player_id][frame];
      game_manager_to_confirm_.SetPlayerInput(input, player_id);
    }

    game_manager_to_confirm_.FixedUpdate();
  }

  return game_manager_to_confirm_.ComputeChecksum();
}

void RollbackManager::ConfirmFrame() noexcept {
 for (int frame = confirmed_frame_ + 1; frame <= frame_to_confirm_; frame++) {
    for (PlayerId player_id = 0; player_id < game_constants::kMaxPlayerCount;
         player_id++) {
      const auto input = inputs_[player_id][frame];
      confirmed_game_manager_.SetPlayerInput(input, player_id);
    }

    confirmed_game_manager_.FixedUpdate();
  }

  confirmed_frame_++;
  frame_to_confirm_++;
}
