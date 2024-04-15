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
  //game_manager_.FixedUpdate();

  auto it = inputs_packet_queue.begin();

  while (it != inputs_packet_queue.end())
  {
    it->delay -= GetFrameTime();

    if (it->delay <= 0.f) {
      const auto other_client_idx = it->client_idx == 1 ? 1 : 0;
      clients_[other_client_idx]->ReceiveInputs(it->frame_inputs);
      it = inputs_packet_queue.erase(it);
    } else
    {
      ++it;
    }
  }

  //for (auto& packet_inputs : inputs_packet_queue) {
  //  
  //  packet_inputs.delay -= GetFrameTime();

  //  if (packet_inputs.delay <= 0.f) {
  //    const auto other_client_idx = packet_inputs.client_idx == 1 ? 1 : 0;
  //    clients_[other_client_idx]->ReceiveInputs(packet_inputs.frame_inputs);
  //    auto it = std::find(inputs_packet_queue.begin(),
  //                        inputs_packet_queue.end(), packet_inputs);
  //    inputs_packet_queue.erase(it);
  //  }
  //}
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

void DebugServer::SendInputs(inputs::FrameInput,
                             DebugClient* client) noexcept {}

void DebugServer::ReceiveInputs(inputs::DebugInput inputs) noexcept {
  if (Math::Random::Range(0.f, 1.f) <= packet_loss_percentage) {
    return;
  }

  inputs_packet_queue.push_back(inputs);
}
