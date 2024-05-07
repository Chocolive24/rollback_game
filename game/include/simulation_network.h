#pragma once

#include "client.h"
#include "input.h"
#include "network_interface.h"

/**
 * \brief SimulationInput is a struct containing a frame input and have a delay
 * value to simulate the network delay.
 */
struct SimulationInput {
  std::vector<input::FrameInput> frame_inputs;
  float delay = 0.f;
};

struct SimulationFrameToConfirm {
  int check_sum = 0;
  std::vector<input::FrameInput> frame_inputs{};
  float delay = 0.f;
};

class SimulationNetwork final : public NetworkInterface {
public:
  void RegisterClient(Client* client) noexcept { client_ = client; }
  void RegisterOtherClientNetwork(SimulationNetwork* other_client_network) noexcept {
   other_client_network_ = other_client_network;
  }

  void Service() noexcept;

  void JoinRandomOrCreateRoom() noexcept override{}
  void LeaveRoom() noexcept override{}

  void RaiseEvent(bool reliable, NetworkEventCode event_code,
    const ExitGames::Common::Hashtable& event_data) noexcept override;
  void ReceiveEvent(int player_nr, NetworkEventCode event_code,
    const ExitGames::Common::Hashtable& event_content) noexcept override;

  static float min_packet_delay;
  static float max_packet_delay;
  static float packet_loss_percentage;

private:
  void PollInputPackets();
  void PollConfirmFramePackets();

  Client* client_ = nullptr;
  SimulationNetwork* other_client_network_ = nullptr;
  std::vector<SimulationInput> waiting_input_queue_{};
  std::vector<SimulationFrameToConfirm> waiting_frame_queue_{};
};