#include "rollback_manager.h"

#include <iostream>

#include "game_manager.h"

void RollbackManager::SetLocalPlayerInput(inputs::FrameInput frame_input,
                                     PlayerId player_id) {
  inputs_[player_id][frame_input.frame_nbr] = frame_input.input;
  current_player_manager_->SetPlayerInput(frame_input.input, player_id);
  current_frame_ = frame_input.frame_nbr;
}

void RollbackManager::SetRemotePlayerInput(
    const std::vector<inputs::FrameInput>& frame_inputs, PlayerId player_id) {

  const auto& latest_frame_input = frame_inputs.back();

  const auto frame_diff = latest_frame_input.frame_nbr - last_remote_input_frame_;

  // We already receive the inputs.
  if (frame_diff < 1)
  {
    return;
  }

  // If we didn't receive some inputs between the last time and the new inputs,
  // iterates over the missing inputs to add them in the inputs array.
  if (frame_diff > 1)
  {
    for (int i = last_remote_input_frame_ + 1; i < latest_frame_input.frame_nbr; i++)
    {
      inputs_[player_id][i] = frame_inputs[i].input;
    }
  }

  // Predicts the remote client would not change its inputs until the current
  // frame to have a realtime simulation.
  for (FrameNbr frame = latest_frame_input.frame_nbr; frame <= current_frame_;
       frame++) {
    inputs_[player_id][frame] = latest_frame_input.input;
  }

  SimulateUntilCurrentFrame();

  last_remote_input_frame_ = latest_frame_input.frame_nbr;
}

void RollbackManager::SimulateUntilCurrentFrame() const noexcept {
  // Copy last confirmed player manager state.
  current_player_manager_->Copy(confirmed_player_manager);

  for (int i = confirmed_frame_ + 1; i <= current_frame_; i++) {
    for (PlayerId player_id = 0; player_id < game_constants::kMaxPlayerCount;
         player_id++) {
      const auto input = inputs_[player_id][i];
      current_player_manager_->SetPlayerInput(input, player_id);
    }

    current_player_manager_->FixedUpdate();
  }
}

uint32_t RollbackManager::SimulateUntilFrameToConfirm(
    FrameNbr frame_to_confirm) noexcept {
  // TODO: Pourquoi je ressimule le jeu ??? peut être pas besoin à voir.
  frame_to_confirm_ = frame_to_confirm;

  player_manager_to_confirm_.Copy(confirmed_player_manager);

  for (int frame = confirmed_frame_ + 1; frame <= frame_to_confirm_; frame++) {
    for (PlayerId player_id = 0; player_id < game_constants::kMaxPlayerCount;
         player_id++) {
      const auto input = inputs_[player_id][frame];
      player_manager_to_confirm_.SetPlayerInput(input, player_id);
    }

    player_manager_to_confirm_.FixedUpdate();
  }

  return player_manager_to_confirm_.ComputeChecksum();
}

void RollbackManager::ConfirmFrame(FrameNbr confirm_frame) noexcept {
  // TODO: peut être faire un rollback si pas le meme checksum au cas ou des
  //  inputs auraient été corrompus.
  // SimulateUntilCurrentFrame();

  for (int frame = confirmed_frame_ + 1; frame <= confirm_frame; frame++) {
    for (PlayerId player_id = 0; player_id < game_constants::kMaxPlayerCount;
         player_id++) {
      const auto input = inputs_[player_id][frame];
      confirmed_player_manager.SetPlayerInput(input, player_id);
    }

    confirmed_player_manager.FixedUpdate();
  }
  confirmed_frame_ = confirm_frame;
  frame_to_confirm_++;

  // std::cout << "confirmed frame: " << confirmed_frame_ << '\n';
}
