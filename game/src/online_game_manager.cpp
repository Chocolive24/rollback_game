#include "online_game_manager.h"
#include "Metrics.h"

void OnlineGameManager::RegisterNetworkInterface(
    NetworkInterface* network_interface) noexcept {
  network_interface_ = network_interface;
}

void OnlineGameManager::Init(int input_profile_id) noexcept {
  rollback_manager_.RegisterGameManager(this);

  constexpr int kStartInputCount = 50;
  frame_inputs_.reserve(kStartInputCount);

  LocalGameManager::Init(input_profile_id);
}

void OnlineGameManager::FixedUpdateCurrentFrame() noexcept {
  rollback_manager_.IncreaseCurrentFrame();

  PollNetworkEvents();
  SendInputEvent();

  for (PlayerId player_id = 0; player_id < game_constants::kMaxPlayerCount;
       player_id++) {
    const auto input = rollback_manager_.GetLastPlayerConfirmedInput(player_id);
    SetPlayerInput(input, player_id);
  }

  LocalGameManager::FixedUpdate();
}

void OnlineGameManager::Deinit() noexcept { LocalGameManager::Deinit(); }

void OnlineGameManager::IncreaseCurrentFrame() noexcept {
  rollback_manager_.IncreaseCurrentFrame();
}

void OnlineGameManager::PollNetworkEvents() noexcept {
  while (!network_event_queue_.empty()) {
    const auto& event = network_event_queue_.front();

    switch (event.code) {
      case NetworkEventCode::kInput:
        OnInputReceived(event.content);
        break;
      case NetworkEventCode::kFrameConfirmation:
        OnFrameConfirmationReceived(event.content);
        break;
      default:
        break;
    }

    network_event_queue_.pop();
  }
}

void OnlineGameManager::SendInputEvent() noexcept {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif  // TRACY_ENABLE

  const auto input = input::GetPlayerInput(input_profile_id_);
  const auto current_frame = rollback_manager_.current_frame();

  const auto pos = game_state_.player_manager.GetPlayerPosition(player_id_);
  const auto dir_to_mouse = input::CalculateDirToMouse(pos, player_id_);
  const input::FrameInput frame_input(dir_to_mouse, current_frame, input);
  
  rollback_manager_.SetLocalPlayerInput(frame_input, player_id_);
  frame_inputs_.push_back(frame_input);

  ExitGames::Common::Hashtable event;
  event.put<nByte, input::FrameInput*>(
      static_cast<nByte>(NetworkEventKey::kPlayerInput), frame_inputs_.data(),
      frame_inputs_.size());

  network_interface_->RaiseEvent(false, NetworkEventCode::kInput, event);
}

void OnlineGameManager::SendFrameConfirmationEvent(
    const std::vector<input::FrameInput>& remote_frame_inputs) noexcept {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif  // TRACY_ENABLE

  auto frame_to_confirm_it = std::find_if(
      remote_frame_inputs.begin(), remote_frame_inputs.end(),
      [this](const input::FrameInput& frame_input) {
        return frame_input.frame_nbr() == rollback_manager_.frame_to_confirm();
      });

  auto end_it = remote_frame_inputs.end();

  const auto current_frame = rollback_manager_.current_frame();

  // If the last remote inputs is greater than the current frame. The end
  // iterator must be equal to the frame input of the local current frame.
  if (remote_frame_inputs.back().frame_nbr() > current_frame) {
    // Get the iterator of the inputs at the current frame to avoid to confirm
    // a frame greater than the local current frame.
    const auto current_frame_it =
        std::find_if(remote_frame_inputs.begin(), remote_frame_inputs.end(),
                     [current_frame](const input::FrameInput& frame_input) {
                       return frame_input.frame_nbr() == current_frame;
                     });

    end_it = current_frame_it;
  }

  while (frame_to_confirm_it != end_it) {
    const auto frame_to_confirm = *frame_to_confirm_it;

    if (frame_to_confirm.frame_nbr() > current_frame) {
      // Tried to confirm a frame greater than the local current frame.
      break;
    }

    const int check_sum = rollback_manager_.ConfirmFrame();

    ExitGames::Common::Hashtable event_check_sum;
    event_check_sum.put(static_cast<nByte>(NetworkEventKey::kCheckSum), check_sum);
    event_check_sum.put(static_cast<nByte>(NetworkEventKey::kPlayerInput),
                        frame_inputs_.data(), frame_inputs_.size());

    network_interface_->RaiseEvent(true, NetworkEventCode::kFrameConfirmation,
                                   event_check_sum);

    ++frame_to_confirm_it;
  }
}

void OnlineGameManager::OnInputReceived(
    const ExitGames::Common::Hashtable& event_content) {
  std::vector<input::FrameInput> remote_frame_inputs{};

  const auto input_value =
      event_content.getValue(static_cast<nByte>(NetworkEventKey::kPlayerInput));

  const input::FrameInput* inputs =
      ExitGames::Common::ValueObject<input::FrameInput*>(input_value)
          .getDataCopy();

  const int inputs_count =
      *ExitGames::Common::ValueObject<input::FrameInput*>(input_value)
           .getSizes();

  remote_frame_inputs.reserve(inputs_count);
  for (int i = 0; i < inputs_count; i++) {
    input::FrameInput frame_input{inputs[i]};
    remote_frame_inputs.push_back(frame_input);
  }

  if (remote_frame_inputs.back().frame_nbr() <
      rollback_manager_.last_remote_input_frame()) {
    // received old input, no need to send confirm packet.
    return;
  }

  const PlayerId other_client_id = player_id_ == 0 ? 1 : 0;
  rollback_manager_.SetRemotePlayerInput(remote_frame_inputs,
                                         other_client_id);

  if (player_id_ == kMasterClientId) {
    SendFrameConfirmationEvent(remote_frame_inputs);
  }

  ExitGames::Common::MemoryManagement::deallocateArray(inputs);
}

void OnlineGameManager::OnFrameConfirmationReceived(
    const ExitGames::Common::Hashtable& event_content) {
  if (player_id_ == kMasterClientId) {
    frame_inputs_.erase(frame_inputs_.begin());
    return;
  }

  Checksum checksum = 0;
  std::vector<input::FrameInput> frame_inputs{};

  const auto checksum_value =
      event_content.getValue(static_cast<nByte>(NetworkEventKey::kCheckSum));
  checksum = ExitGames::Common::ValueObject<int>(checksum_value).getDataCopy();

  const auto input_value =
      event_content.getValue(static_cast<nByte>(NetworkEventKey::kPlayerInput));

  const input::FrameInput* inputs =
      ExitGames::Common::ValueObject<input::FrameInput*>(input_value)
          .getDataCopy();

  const int inputs_count =
      *ExitGames::Common::ValueObject<input::FrameInput*>(input_value).getSizes();

  frame_inputs.reserve(inputs_count);
  for (int i = 0; i < inputs_count; i++) {
    input::FrameInput frame_input = inputs[i];
    frame_inputs.push_back(frame_input);
  }

  // If we did not receive the inputs before the frame to confirm, add them.
  if (rollback_manager_.last_remote_input_frame() <
      frame_inputs.back().frame_nbr()) {
    const PlayerId other_client_id = player_id_ == 0 ? 1 : 0;
    rollback_manager_.SetRemotePlayerInput(frame_inputs,
                                           other_client_id);
  }

  const int check_sum = rollback_manager_.ConfirmFrame();

  if (check_sum != checksum) {
    std::cerr << "Not same checksum for frame: "
              << rollback_manager_.frame_to_confirm() << '\n';
    return;
  }

  // Send a frame confirmation event with empty data to the master client
  // just to tell him that we confirmed the frame and that he can erase
  // the input at the confirmed frame in its vector of inputs.
  network_interface_->RaiseEvent(true, NetworkEventCode::kFrameConfirmation,
                                 ExitGames::Common::Hashtable());

  frame_inputs_.erase(frame_inputs_.begin());
  ExitGames::Common::MemoryManagement::deallocateArray(inputs);
}
