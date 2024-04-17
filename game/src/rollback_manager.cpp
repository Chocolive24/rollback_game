#include "rollback_manager.h"

#include <iostream>

#include "game_manager.h"

void RollbackManager::SetPlayerInput(inputs::FrameInput frame_input,
                                     PlayerId player_id) {
  inputs_[player_id][frame_input.frame_nbr] = frame_input.input;
  current_player_manager_->SetPlayerInput(frame_input.input, player_id);
  current_frame_ = frame_input.frame_nbr;
}

// void RollbackManager::SetRemotePlayerInput(inputs::FrameInput frame_input,
//                                            PlayerId player_id) {
//   for (FrameNbr frame = last_remote_input_frame_ + 1; frame <=
//   current_frame_; frame++) {
//     inputs_[player_id][frame] = frame_input.input;
//   }
//
//   SimulateUntilCurrentFrame();
//
//   // if no need to rollback.
//   //current_player_manager_->SetPlayerInput(frame_input.input, player_id);
//
//
//   last_remote_input_frame_ = frame_input.frame_nbr;
// }

void RollbackManager::SetRemotePlayerInput(
    const std::vector<inputs::FrameInput>& frame_inputs, PlayerId player_id) {

  const auto latest_frame_input = frame_inputs.back();

  const auto fram_diff = latest_frame_input.frame_nbr - last_remote_input_frame_;

  // We already receive the inputs.
  if (fram_diff < 1)
  {
    return;
  }

  // If we didn't receive some inputs between the last time and the new inputs,
  // iterates over the missing inputs to add them in the inputs array.
  if (fram_diff > 1)
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

  //if (inputs_[player_id][latest_frame_input.frame_nbr] != latest_frame_input.input)
  //{
  //  SimulateUntilCurrentFrame();
  //}
  //else
  //{
  //  // if no need to rollback.
  //  current_player_manager_->SetPlayerInput(latest_frame_input.input,
  //                                          player_id);
  //}


  last_remote_input_frame_ = latest_frame_input.frame_nbr;

  ConfirmFrame(last_remote_input_frame_);
}

void RollbackManager::SimulateUntilCurrentFrame() noexcept {
  // Copy last confirmed player manager state.
  current_player_manager_->Copy(confirmed_player_manager);

  // For each frame to catch up.
  // TODO: av: i = last_remote_input_frame + 1
  for (int i = confirmed_frame_ + 1; i <= current_frame_; i++) {
    for (PlayerId player_id = 0; player_id < game_constants::kMaxPlayerCount;
         player_id++) {
      const auto input = inputs_[player_id][i];
      current_player_manager_->SetPlayerInput(input, player_id);
    }

    current_player_manager_->FixedUpdate();
  }

  //confirmed_player_manager.Copy(*current_player_manager_);
}

uint32_t RollbackManager::SimulateUntilFrameToConfirm(
    FrameNbr frame_to_confirm) noexcept {
  // TODO: Pourquoi je ressimule le jeu ??? peut �tre pas besoin � voir.
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
  // TODO: peut �tre faire un rollback si pas le meme checksum au cas ou des
  //  inputs auraient �t� corrompus.
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

  // std::cout << "confirmed frame: " << confirmed_frame_ << '\n';
}
