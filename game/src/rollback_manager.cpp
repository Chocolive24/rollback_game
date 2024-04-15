#include "rollback_manager.h"

#include <iostream>

#include "game_manager.h"

//inputs::PlayerInput RollbackManager::GetPlayerInputAtFrame(
//	FrameNbr frame_nbr, PlayerId player_id) noexcept {
//
// // if (inputs_[player_id].empty()) {
// //   return 0;
// // }
//
// // if (frame_nbr > last_remote_input_frame) {
// //     inputs_[player_id].push_back(inputs_[player_id].back());
// //     return inputs_[player_id].back();
// // }
//
//  return inputs_[player_id][frame_nbr];
//}

void RollbackManager::SetPlayerInput(inputs::FrameInput frame_input, PlayerId player_id) {
  inputs_[player_id][frame_input.frame_nbr] = frame_input.input;
  current_player_manager_->SetPlayerInput(frame_input.input, player_id);
  current_frame_ = frame_input.frame_nbr;
}

void RollbackManager::SetRemotePlayerInput(inputs::FrameInput frame_input,
                                           PlayerId player_id) {
  //if (last_remote_input_frame < 0) {
    for (FrameNbr frame = last_remote_input_frame + 1; frame <= current_frame_; frame++) {
      inputs_[player_id][frame] = frame_input.input;
    }

    SimulateUntilCurrentFrame();

  //  return;
  //}

  //if (frame_input.input == inputs_[player_id][last_remote_input_frame]) {
  //  inputs_[player_id].push_back(frame_input.input);
  //  current_player_manager_->SetPlayerInput(frame_input.input, player_id);
  //  last_remote_input_frame = frame_input.frame_nbr;

  //  return;
  //}

  //On remplit les inputs du remote client avec l'input qu'on vient de recevoir car on predit
  //qu'il ne changera pas d'input.
  //TODO: A FAIRE QUAND LES INPUTS AURONT UN DELAIS ALEATOIRE.
  //for (FrameNbr frame = last_remote_input_frame + 1; frame <= current_frame_;
  //     frame++) {
  //  inputs_[player_id].push_back(frame_input.input);
  //}

  //// On resimule le jeu avec le nouvel input.
  //SimulateUntilCurrentFrame();

  last_remote_input_frame = frame_input.frame_nbr;
}

void RollbackManager::SimulateUntilCurrentFrame() noexcept {
   // Copy last confirmed player manager state.
  current_player_manager_->Copy(confirmed_player_manager);
  bool test = false;
  // For each frame to catch up.
  for (int i = last_remote_input_frame + 1; i <= current_frame_; i++) {
    for (PlayerId player_id = 0; player_id < game_constants::kMaxPlayerCount; player_id++)
    {
      const auto input = inputs_[player_id][i];
      current_player_manager_->SetPlayerInput(input, player_id);
    }

    current_player_manager_->FixedUpdate();

    if (!test)
    {
      confirmed_player_manager.Copy(*current_player_manager_);
      test = true;
    }
  }

  //confirmed_player_manager.Copy(*current_player_manager_);
  //last_remote_input_frame = current_frame_;
}
