#pragma once

#include "game_manager.h"
#include "network_interface.h"
#include "rollback_manager.h"

/**
 * \brief NetworkGameManager is a class which is a GameManager with a network
 * interface to send events over the network.
 */
class NetworkGameManager final : public GameManager {
public:
  void RegisterNetworkInterface(NetworkInterface* network_interface) noexcept;

  void SendInputEvent() noexcept;
  void OnEventReceived(EventCode event_code, 
      const ExitGames::Common::Hashtable& event_content) noexcept;

private:
  void SendFrameConfirmationEvent(
      const std::vector<inputs::FrameInput>& remote_frame_inputs) noexcept;

  std::vector<inputs::PlayerInput> inputs_{};
  std::vector<FrameNbr> frames_{};

  NetworkInterface* network_interface_ = nullptr;

  static constexpr PlayerId kMasterClientId = 0;
};