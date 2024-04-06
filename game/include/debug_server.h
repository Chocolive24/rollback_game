#pragma once

#include "game_manager.h"
#include "inputs_manager.h"

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

  void SendInputs(inputs::FrameInputs inputs, DebugClient* client) noexcept;
  void ReceiveInputs(inputs::FrameInputs inputs, int client_idx) noexcept;

private:
 GameManager game_manager_{};
 std::array<DebugClient*, game_constants::kMaxPlayerCount> clients_{};

 float delay_ = 0.1f;
 float delay_time_ = 0.f;
 float packet_loss_percentage_ = 0.10f;
};
