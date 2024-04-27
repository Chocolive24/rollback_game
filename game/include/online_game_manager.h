#pragma once

#include <queue>

#include "local_game_manager.h"
#include "network_interface.h"
#include "rollback_manager.h"

/**
 * \brief OnlineGameManager is a class that is a LocalGameManager adding features
 * for playing online.
 *
 * The added online layer includes a pointer to a network interface for sending
 * and receiving events, and a rollback manager to maintain the integrity
 * of the game simulation.
 */
class OnlineGameManager final : public LocalGameManager {
public:
  void RegisterNetworkInterface(NetworkInterface* network_interface) noexcept;

  void Init(int input_profile_id) noexcept override;
  void FixedUpdateCurrentFrame() noexcept;

  void IncreaseCurrentFrame() noexcept;
  void OnInputReceived(const ExitGames::Common::Hashtable& event_content);
  void OnFrameConfirmationReceived(const ExitGames::Common::Hashtable& event_content);

  void SendInputEvent() noexcept;
  void PushNetworkEvent(const NetworkEvent& network_event) noexcept {
    network_event_queue_.push(network_event);
  }
  void OnEventReceived(NetworkEventCode event_code, 
      const ExitGames::Common::Hashtable& event_content) noexcept;

private:
  void PollNetworkEvents() noexcept;
  void SendFrameConfirmationEvent(
      const std::vector<inputs::FrameInput>& remote_frame_inputs) noexcept;

  std::queue<NetworkEvent> network_event_queue_{};

  std::vector<inputs::PlayerInput> inputs_{};
  std::vector<FrameNbr> frames_{};

  RollbackManager rollback_manager_;
  NetworkInterface* network_interface_ = nullptr;

  static constexpr PlayerId kMasterClientId = 0;
};