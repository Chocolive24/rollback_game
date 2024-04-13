#pragma once

#include "game_manager.h"
#include "inputs.h"

#include <vector>

class DebugClient;

class DebugServer {
public:
  void RegisterClients(DebugClient* client_1, DebugClient* client_2) noexcept {
   clients_[0] = client_1;
   clients_[1] = client_2;
  }
  void Setup() noexcept;
  void Update() noexcept;
  void DrawImGui() noexcept;
  void TearDown() noexcept;

  void SendInputs(inputs::FrameInput inputs, DebugClient* client) noexcept;
  void ReceiveInputs(inputs::DebugInput inputs) noexcept;

  std::vector<inputs::DebugInput> inputs_packet_queue{};
  float min_packet_delay = 0.1f;
  float max_packet_delay = 0.3f;
  float packet_loss_percentage = 0.1f;

private:
 GameManager game_manager_{};

 std::array<DebugClient*, game_constants::kMaxPlayerCount> clients_{};
};
