#include "debug_server.h"

#include <imgui.h>
#include <iostream>
#include <string>

#include "debug_client.h"
#include "Random.h"

void DebugServer::Setup() noexcept {
  //game_manager_.Init();
  inputs_packet_queue.reserve(1000);
}

void DebugServer::Update() noexcept {
  //game_manager_.Update();

  for (auto& packet_inputs : inputs_packet_queue) {
    
    packet_inputs.delay -= GetFrameTime();

    if (packet_inputs.delay <= 0.f) {
      const auto other_client_idx = packet_inputs.client_idx == 1 ? 1 : 0;
      clients_[other_client_idx]->ReceiveInputs(packet_inputs.frame_inputs);
    }
  }
}

void DebugServer::DrawImGui() noexcept {
  ImGui::Begin("Mock network values."); {
    ImGui::SliderFloat("Min delay", &min_packet_delay, 0.01f, 1.f);
    ImGui::SliderFloat("Max delay", &max_packet_delay, 0.01f, 1.f);
    ImGui::SliderFloat("PacketLossPercentage", &packet_loss_percentage, 0.f, 1.f);
  }
  ImGui::End();
}

void DebugServer::TearDown() noexcept {
  //game_manager_.Deinit();
}

void DebugServer::SendInputs(inputs::FrameInputs,
                             DebugClient* client) noexcept {}

void DebugServer::ReceiveInputs(inputs::DebugInputs inputs) noexcept {
  if (Math::Random::Range(0.f, 1.f) <= packet_loss_percentage) {
    return;
  }

  inputs_packet_queue.push_back(inputs);
}
